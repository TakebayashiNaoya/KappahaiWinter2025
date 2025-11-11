#include "stdafx.h"
#include "CollisionManager.h"

#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"
#include "Source/Scene/SceneManager.h"


CollisionHitManager* CollisionHitManager::m_instance = nullptr;

namespace
{
	// プレイヤーが無敵中か、プレイヤーの攻撃が先に当たっている場合、trueを返す。
	const bool IsAttackBlocked(Player* player, const bool isStomp)
	{
		if (player->GetIsInvincible()) {
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
		if (UpdateHitPlayerTransformEnemy(pair)) {
			continue;
		}

		// 基本エネミー vs 変形エネミー
		if (UpdateHitBasicEnemyTransformEnemy(pair)) {
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
///　「プレイヤー」と「基本エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitPlayerBasicEnemy(CollisionPair& pair)
{
	Player* player = GetTargetObject<Player>(pair, enCollisionType_Player);
	BasicEnemy* basicEnemy = GetTargetObject<BasicEnemy>(pair, enCollisionType_BasicEnemy);


	if (player == nullptr) {
		return false;
	}
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
	if (player->GetIsInvincible()) {
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
///　「プレイヤー」と「変形エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitPlayerTransformEnemy(CollisionPair& pair)
{
	Player* player = GetTargetObject<Player>(pair, enCollisionType_Player);
	TransformEnemy* transformEnemy = GetTargetObject<TransformEnemy>(pair, enCollisionType_TransformEnemy);


	if (player == nullptr) {
		return false;
	}
	if (transformEnemy == nullptr) {
		return false;
	}


	// エネミーが変形していない場合。
	if (!transformEnemy->GetIsTransform())
	{
		// プレイヤーの攻撃。
		if (player->GetStompCollider()->IsHit(transformEnemy->GetBodyCollider())) {
			player->StompJump();
			transformEnemy->SetIsTransform(true);
			return true;
		}

		// プレイヤーが無敵中の場合、またはプレイヤーの攻撃が先に当たっている場合、エネミーの攻撃は無効にする。
		if (player->GetIsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (transformEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(transformEnemy->GetPosition());
			return true;
		}

		return true;
	}

	// エネミーが変形していて、滑走していない場合。
	else if (transformEnemy->GetIsTransform() && !transformEnemy->GetIsSliding())
	{
		// プレイヤーがエネミーに当たった場合。
		if (player->GetBodyCollider()->IsHit(transformEnemy->GetBodyCollider())) {
			transformEnemy->SetIsSliding(true);
			transformEnemy->CalcInitialSlideDirection(player->GetPosition());
			return true;
		}
		// プレイヤーがエネミーを踏んだ場合。
		if (player->GetStompCollider()->IsHit(transformEnemy->GetBodyCollider())) {
			player->StompJump();
			transformEnemy->SetIsSliding(true);
			transformEnemy->CalcInitialSlideDirection(player->GetPosition());
		}
		return true;
	}

	// エネミーが変形していて、滑走している場合。
	else if (transformEnemy->GetIsTransform() && transformEnemy->GetIsSliding())
	{
		// 滑走中にプレイヤーが踏んだら、エネミーを止める。
		if (player->GetStompCollider()->IsHit(transformEnemy->GetBodyCollider())) {
			player->StompJump();
			transformEnemy->SetIsSliding(false);
			return true;
		}

		// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
		if (player->GetIsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (transformEnemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(transformEnemy->GetPosition());
			transformEnemy->SetIsDead(true);
			return true;
		}
		return true;
	}

	return true;
}

/// <summary>
///　「基本エネミー」と「変形エネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitBasicEnemyTransformEnemy(CollisionPair& pair)
{
	BasicEnemy* basicEnemy = GetTargetObject<BasicEnemy>(pair, enCollisionType_BasicEnemy);
	TransformEnemy* transformEnemy = GetTargetObject<TransformEnemy>(pair, enCollisionType_TransformEnemy);


	if (basicEnemy == nullptr) {
		return false;
	}
	if (transformEnemy == nullptr) {
		return false;
	}


	// 変形エネミーが変形していて、滑走している場合。
	if (transformEnemy->GetIsTransform() && transformEnemy->GetIsSliding())
	{
		// 変形エネミーの攻撃。
		if (transformEnemy->GetBodyCollider()->IsHit(basicEnemy->GetBodyCollider())) {
			basicEnemy->SetIsDead(true);
			transformEnemy->SetIsDead(true);
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