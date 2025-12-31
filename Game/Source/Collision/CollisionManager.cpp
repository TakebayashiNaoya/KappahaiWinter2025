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
	m_collisionPairList.clear();
}


void CollisionHitManager::Update()
{
	// シーン切り替えがリクエストされている場合は、現在のフレームの衝突判定処理をスキップする。
	// これにより、削除が始まったオブジェクトへの不正アクセスを防ぐ。
	if (SceneManager::GetInstance()->GetIsSceneChangeRequested()) {
		m_collisionPairList.clear(); // 念のためリストはクリア
		return;
	}

	if (BattleManager::GetIsStopCollisionManager()) {
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

		// 変形エネミー vs ボスエネミー
		if (UpdateHitDeformEnemyBossEnemy(pair)) {
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


	// プレイヤーの攻撃。
	if (player->GetAttackCollider()->IsHit(basicEnemy->GetHurtCollider())) {
		player->StompJump();
		basicEnemy->SetIsDying(true);
		SoundManager::Play(enSoundList_Stomp);
		return true;
	}

	// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
	if (player->GetIsInvincible()) {
		return true;
	}

	// エネミーの攻撃。
	if (basicEnemy->GetHitCollider()->IsHit(player->GetHurtCollider())) {
		player->SetIsAttacked(true);
		player->ComputeKnockBackDirection(basicEnemy->GetPosition());
		basicEnemy->SetIsCoolDown(true);
		SoundManager::Play(enSoundList_PlayerDamage);
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

	DeformEnemy* deformEnemy = GetTargetObject<DeformEnemy>(pair, enCollisionType_DeformEnemy);
	if (deformEnemy == nullptr) {
		return false;
	}


	// エネミーが変形していない場合。
	if (!deformEnemy->GetIsDeformed())
	{
		// プレイヤーの攻撃。
		if (player->GetAttackCollider()->IsHit(deformEnemy->GetHurtCollider())) {
			player->StompJump();
			deformEnemy->SetIsDeformed(true);
			SoundManager::Play(enSoundList_Stomp);
			return true;
		}

		// プレイヤーが無敵中の場合、またはプレイヤーの攻撃が先に当たっている場合、エネミーの攻撃は無効にする。
		if (player->GetIsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (deformEnemy->GetHitCollider()->IsHit(player->GetHurtCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(deformEnemy->GetPosition());
			SoundManager::Play(enSoundList_PlayerDamage);
			return true;
		}

		return true;
	}

	// エネミーが変形していて、滑走していない場合。
	else if (deformEnemy->GetIsDeformed() && !deformEnemy->GetIsSliding())
	{
		// プレイヤーがエネミーに当たった場合。
		if (player->GetHurtCollider()->IsHit(deformEnemy->GetHurtCollider())) {
			deformEnemy->SetIsSliding(true);
			deformEnemy->CalcInitialSlideDirection(player->GetPosition());
			SoundManager::Play(enSoundList_SlidingStart);
			return true;
		}
		return true;
	}

	// エネミーが変形していて、滑走している場合。
	else if (deformEnemy->GetIsDeformed() && deformEnemy->GetIsSliding())
	{
		// 滑走中にプレイヤーが踏んだら、エネミーを止める。
		if (player->GetAttackCollider()->IsHit(deformEnemy->GetHurtCollider())) {
			player->StompJump();
			deformEnemy->SetIsSliding(false);
			SoundManager::Play(enSoundList_Stomp);
			return true;
		}

		// プレイヤーが無敵中の場合、エネミーの攻撃は無効にする。
		if (player->GetIsInvincible()) {
			return true;
		}

		// エネミーの攻撃。
		if (deformEnemy->GetHitCollider()->IsHit(player->GetHurtCollider())) {
			player->SetIsAttacked(true);
			player->ComputeKnockBackDirection(deformEnemy->GetPosition());
			deformEnemy->SetIsDying(true);
			SoundManager::Play(enSoundList_PlayerDamage);
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
	if (player->GetIsInvincible()) {
		return true;
	}


	// ボスの体当たり。
	if (bossEnemy->GetHitCollider()->IsHit(player->GetHurtCollider())) {
		player->SetIsAttacked(true);
		player->ComputeKnockBackDirection(bossEnemy->GetPosition());
		SoundManager::Play(enSoundList_PlayerDamage);
		return true;
	}

	// ボスの攻撃。
	if (bossEnemy->GetAttackCollider()->IsHit(player->GetHurtCollider())) {
		player->SetIsAttacked(true);
		player->ComputeKnockBackDirection(bossEnemy->GetPosition());
		SoundManager::Play(enSoundList_PlayerDamage);
		return true;
	}

	return true;
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

	DeformEnemy* deformEnemy = GetTargetObject<DeformEnemy>(pair, enCollisionType_DeformEnemy);
	if (deformEnemy == nullptr) {
		return false;
	}


	// 変形エネミーが変形していて、滑走している場合。
	if (deformEnemy->GetIsDeformed() && deformEnemy->GetIsSliding())
	{
		// 変形エネミーの攻撃。
		if (deformEnemy->GetHitCollider()->IsHit(basicEnemy->GetHurtCollider())) {
			basicEnemy->SetIsDying(true);
			deformEnemy->SetIsDying(true);
			SoundManager::Play(enSoundList_Stomp);
			return true;
		}
		return true;
	}
	return true;
}

/// <summary>
/// 「変形エネミー」と「ボスエネミー」の衝突処理を行います。
/// </summary>
bool CollisionHitManager::UpdateHitDeformEnemyBossEnemy(CollisionPair& pair)
{
	DeformEnemy* deformEnemy = GetTargetObject<DeformEnemy>(pair, enCollisionType_DeformEnemy);
	if (deformEnemy == nullptr) {
		return false;
	}

	BossEnemy* bossEnemy = GetTargetObject<BossEnemy>(pair, enCollisionType_BossEnemy);
	if (bossEnemy == nullptr) {
		return false;
	}

	// 変形エネミーが変形していて、滑走している場合。
	if (deformEnemy->GetIsDeformed() && deformEnemy->GetIsSliding())
	{
		// 変形エネミーの攻撃。
		if (deformEnemy->GetHitCollider()->IsHit(bossEnemy->GetHurtCollider())) {
			bossEnemy->SetIsAttacked(true);
			deformEnemy->SetIsDying(true);
			SoundManager::Play(enSoundList_Stomp);
			return true;
		}
		return true;
	}

	return true;
}




/********************************/


CollisionObject* CollisionHitManager::CreateCollider(
	Character* ins, const EnCollisionType type, const Vector3 size, const bool isTrigger)
{
	// ゴーストオブジェクトを作成。
	CollisionObject* collider = new CollisionObject();
	collider->CreateBox(
		ins->GetPosition(),
		ins->GetRotation(),
		size
	);

	// コリジョンヒットマネージャーに登録。
	m_instance->Register(type, collider, ins);

	// RayTestで無視するかどうかを設定。
	m_instance->SetIsTrigger(collider, isTrigger);

	return collider;
}


CollisionObject* CollisionHitManager::CreateCollider(
	Character* ins, const EnCollisionType type, const float radius, const int index)
{
	// ゴーストオブジェクトを作成。
	CollisionObject* collider = new CollisionObject();
	collider->CreateSphere(
		ins->GetPosition(),
		ins->GetRotation(),
		radius
	);

	// コリジョンヒットマネージャーに登録。
	m_instance->Register(type, collider, ins);

	// RayTestで無視するかどうかを設定。
	m_instance->SetIsTrigger(collider, index);

	return collider;
}


CollisionObject* CollisionHitManager::CreateCollider(
	Character* ins, const EnCollisionType type, const float radius, const float height, const bool isTrigger)
{
	// ゴーストオブジェクトを作成。
	CollisionObject* collider = new CollisionObject();
	collider->CreateCapsule(
		ins->GetPosition(),
		ins->GetRotation(),
		radius,
		height
	);

	// コリジョンヒットマネージャーに登録。
	m_instance->Register(type, collider, ins);

	// RayTestで無視するかどうかを設定。
	m_instance->SetIsTrigger(collider, isTrigger);

	return collider;
}


void CollisionHitManager::UpdateCollider(const Character* ins, CollisionObject* collider, const float offset)
{
	if (collider == nullptr) {
		return;
	}

	// コライダーの座標を計算する。
	Vector3 ghostPos = ins->GetPosition() + ins->GetUpDirection() * offset;

	// コライダーの座標をモデルの座標に合わせる。
	collider->SetPosition(ghostPos);

	// コライダーの回転をモデルの回転に合わせる。
	collider->SetRotation(ins->GetRotation());
}


/// <summary>
/// やられ判定をdelete、nullptrします。
/// </summary>
CollisionObject* CollisionHitManager::DeleteCollider(CollisionObject* collider)
{
	if (collider == nullptr) {
		return nullptr;
	}

	// コリジョンヒットマネージャーから登録解除。
	if (GetIsAvailable()) {
		GetInstance()->Unregister(collider);
	}

	delete collider;
	return nullptr;
}

void CollisionHitManager::SetIsTrigger(CollisionObject* collider, int index)
{
	if (collider) {
		// true(トリガー)なら 1、false(通常)なら 0 をセット
		collider->GetbtCollisionObject().setUserIndex(index);
	}
}




/********************************/


CollisionManagerObject::CollisionManagerObject()
{
	CollisionHitManager::CreateInstance();
}


CollisionManagerObject::~CollisionManagerObject()
{
	CollisionHitManager::Delete();
}


bool CollisionManagerObject::Start()
{
	return true;
}


void CollisionManagerObject::Update()
{
	CollisionHitManager::GetInstance()->Update();
}