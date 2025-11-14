#include "stdafx.h"
#include "CollisionManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/Scene/SceneManager.h"


CollisionHitManager* CollisionHitManager::m_instance = nullptr;

namespace
{
	// プレイヤーが無敵中か、プレイヤーの攻撃が先に当たっている場合、trueを返す。
	const bool IsAttackBlocked(Player* player, const bool isStomp)
	{
		if (player->IsInvincible()) {
			return true;
		}
		else if (isStomp) {
			return true;
		}
		return false;
	}
}


CollisionHitManager::CollisionHitManager()
{
	m_collisionInformationList.clear();
}


CollisionHitManager::~CollisionHitManager()
{
	m_collisionInformationList.clear();
}


void CollisionHitManager::Update()
{
	// シーン切り替えがリクエストされている場合は、現在のフレームの衝突判定処理をスキップする。
	// これにより、削除が始まったオブジェクトへの不正アクセスを防ぐ。
	if (SceneManager::GetInstance()->IsSceneChangeRequested()) {
		m_collisionPairList.clear(); // 念のためリストはクリア
		return;
	}

	m_collisionPairList.clear();

	// ヒットするオブジェクトのペアを作る
	const uint32_t colSize = static_cast<uint32_t>(m_collisionInformationList.size());
	for (uint32_t i = 0; i < colSize; ++i) {
		for (uint32_t j = i + 1; j < colSize; ++j) {
			CollisionInformation* infoA = &m_collisionInformationList[i];
			CollisionInformation* infoB = &m_collisionInformationList[j];

			if (infoA->m_collision->IsHit(infoB->m_collision) || infoB->m_collision->IsHit(infoA->m_collision))
			{
				// CollisionPairの中に同じ組み合わせがないかチェック
				bool exists = false;
				for (const auto& pair : m_collisionPairList) {
					if ((pair.m_left == infoA && pair.m_right == infoB) || (pair.m_left == infoB && pair.m_right == infoA)) {
						exists = true;
						break;
					}
				}
				// すでに登録済みではないなら追加する
				if (!exists) {
					m_collisionPairList.push_back(CollisionPair(infoA, infoB));
				}
			}
		}
	}

	// ヒットしたペアで衝突した時の処理をする
	// 今回のゲームではないがプレイヤーの攻撃がエネミーにあたったのでHPを減らすみたいなことをする
	for (auto& pair : m_collisionPairList) {

		// プレイヤー vs 基本エネミー
		if (UpdateHitPlayerBasicEnemy(pair)) {
			continue;
		}

		// プレイヤー vs 変形エネミー
		if (UpdateHitPlayerDeformEnemy(pair)) {
			continue;
		}

		// プレイヤー vs ボスエネミー
		if (UpdateHitPlayerBossEnemy(pair)) {
			continue;
		}

		// 基本エネミー vs 変形エネミー
		if (UpdateHitBasicEnemyDeformEnemy(pair)) {
			continue;
		}
	}

	m_collisionPairList.clear();
}


void CollisionHitManager::Register(const EnCollisionType type, CollisionObject* collisionObject, IGameObject* gameObject)
{
	CollisionInformation info(type, collisionObject, gameObject);
	m_collisionInformationList.push_back(info);
}


void CollisionHitManager::Unregister(CollisionObject* collisionObject)
{
	if (m_collisionInformationList.size() == 0) {
		return;
	}

	for (auto it = m_collisionInformationList.begin(); it != m_collisionInformationList.end(); ++it)
	{
		if (it->m_collision == nullptr) {
			continue;
		}

		if (it->m_collision == collisionObject) {
			m_collisionInformationList.erase(it);
			break;
		}
	}
}


/// <summary>
/// 「プレイヤー」と「基本エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitPlayerBasicEnemy(CollisionPair& pair)
{
	Player* player = GetTargetObject<Player>(pair, enCollisionType_Player);
	if (player == nullptr) {
		return false;
	}

	BasicEnemy* basicEnemy = GetTargetObject<BasicEnemy>(pair, enCollisionType_BasicEnemy);
	if (basicEnemy == nullptr) {
		return false;
	}


	// エネミー死亡時、コライダーを消してから一定時間後にモデルを消しているため、
	// コライダーがnullptrの場合はスキップする。
	if (basicEnemy->GetBodyCollider() == nullptr) {
		return true;
	}

	// プレイヤーの攻撃。
	if (player->GetStompCollider()->IsHit(basicEnemy->GetBodyCollider())) {
		player->StompJump();
		basicEnemy->SetIsDead(true);
		return true;
	}

	// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
	if (player->IsInvincible()) {
		return true;
	}

	// エネミーの攻撃。
	if (basicEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
		player->SetIsAttacked(true);
		player->ComputeKnockBackDirection(basicEnemy->GetPosition());
		basicEnemy->SetIsCoolDown(true);
		return true;
	}

	return true;
}

/// <summary>
/// 「プレイヤー」と「変形エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitPlayerDeformEnemy(CollisionPair& pair)
{
	Player* player = GetTargetObject<Player>(pair, enCollisionType_Player);
	if (player == nullptr) {
		return false;
	}

	DeformEnemy* deformEnemy = GetTargetObject<DeformEnemy>(pair, enCollisionType_TransformEnemy);
	if (deformEnemy == nullptr) {
		return false;
	}


	// エネミーが変形していない場合。
	if (!deformEnemy->IsDeformed())
	{
		// プレイヤーの攻撃。
		if (player->GetStompCollider()->IsHit(deformEnemy->GetBodyCollider())) {
			player->StompJump();
			deformEnemy->SetIsDeformed(true);
			return true;
		}

		// プレイヤーが無敵中の場合、またはプレイヤーの攻撃が先に当たっている場合、エネミーの攻撃は無効にする。
		if (player->IsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (deformEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(deformEnemy->GetPosition());
			return true;
		}

		return true;
	}

	// エネミーが変形していて、滑走していない場合。
	else if (deformEnemy->IsDeformed() && !deformEnemy->IsSliding())
	{
		// プレイヤーがエネミーに当たった場合。
		if (player->GetBodyCollider()->IsHit(deformEnemy->GetBodyCollider())) {
			deformEnemy->SetIsSliding(true);
			deformEnemy->CalcInitialSlideDirection(player->GetPosition());
			return true;
		}
		// プレイヤーがエネミーを踏んだ場合。
		if (player->GetStompCollider()->IsHit(deformEnemy->GetBodyCollider())) {
			player->StompJump();
			deformEnemy->SetIsSliding(true);
			deformEnemy->CalcInitialSlideDirection(player->GetPosition());
		}
		return true;
	}

	// エネミーが変形していて、滑走している場合。
	else if (deformEnemy->IsDeformed() && deformEnemy->IsSliding())
	{
		// 滑走中にプレイヤーが踏んだら、エネミーを止める。
		if (player->GetStompCollider()->IsHit(deformEnemy->GetBodyCollider())) {
			player->StompJump();
			deformEnemy->SetIsSliding(false);
			return true;
		}

		// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
		if (player->IsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (deformEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(deformEnemy->GetPosition());
			deformEnemy->SetIsDead(true);
			return true;
		}
		return true;
	}

	return true;
}

/// <summary>
/// 「プレイヤー」と「ボスエネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitPlayerBossEnemy(CollisionPair& pair)
{
	Player* player = GetTargetObject<Player>(pair, enCollisionType_Player);
	if (player == nullptr) {
		return false;
	}

	BossEnemy* bossEnemy = GetTargetObject<BossEnemy>(pair, enCollisionType_BossEnemy);
	if (bossEnemy == nullptr) {
		return false;
	}


	// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
	if (player->IsInvincible()) {
		return true;
	}


	// ボスの攻撃。
	if (bossEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
		player->SetIsAttacked(true);
		player->ComputeKnockBackDirection(bossEnemy->GetPosition());
		return true;
	}
}

/// <summary>
/// 「基本エネミー」と「変形エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitBasicEnemyDeformEnemy(CollisionPair& pair)
{
	BasicEnemy* basicEnemy = GetTargetObject<BasicEnemy>(pair, enCollisionType_BasicEnemy);
	if (basicEnemy == nullptr) {
		return false;
	}

	DeformEnemy* deformEnemy = GetTargetObject<DeformEnemy>(pair, enCollisionType_TransformEnemy);
	if (deformEnemy == nullptr) {
		return false;
	}


	// 変形エネミーが変形していて、滑走している場合。
	if (deformEnemy->IsDeformed() && deformEnemy->IsSliding())
	{
		// 変形エネミーの攻撃。
		if (deformEnemy->GetBodyCollider()->IsHit(basicEnemy->GetBodyCollider())) {
			basicEnemy->SetIsDead(true);
			deformEnemy->SetIsDead(true);
			return true;
		}
		return true;
	}
	return true;
}




/********************************/


CollisionManagerObject::CollisionManagerObject()
{
	m_collisionHitManager = CollisionHitManager::CreateInstance();
}


CollisionManagerObject::~CollisionManagerObject()
{
	CollisionHitManager::Delete();
	m_collisionHitManager = nullptr;
}


bool CollisionManagerObject::Start()
{
	return true;
}


void CollisionManagerObject::Update()
{
	m_collisionHitManager->Update();
}