#include "stdafx.h"
#include "CollisionManager.h"

#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"



CollisionHitManager* CollisionHitManager::m_instance = nullptr;


void CollisionHitManager::Update()
{
	// キャラの数は少なく、FindGOs内で最適化されているため、毎フレーム取得しても問題ないと判断。
	Player* player = FindGO<Player>("Player");
	std::vector<BasicEnemy*> basicEnemys = FindGOs<BasicEnemy>("BasicEnemy");
	std::vector<TransformEnemy*> transformEnemys = FindGOs<TransformEnemy>("TransformEnemy");



	// プレイヤーとベーシックエネミー。
	for (auto* enemy : basicEnemys) {

		// エネミー死亡時、コライダーを消してから一定時間後にモデルを消しているため、
		// コライダーがnullptrの場合はスキップする。
		if (enemy->GetBodyCollider() == nullptr) {
			continue;
		}

		// プレイヤーの攻撃。
		bool successStomp = false;
		if (player->GetStompCollider()->IsHit(enemy->GetBodyCollider())) {
			player->StompJump();
			enemy->SetIsDead(true);
			successStomp = true;
		}

		// プレイヤーが無敵中の場合、またはプレイヤーの攻撃が先に当たっている場合、エネミーの攻撃は無効にする。
		if (player->GetIsInvincible()) {
			continue;
		}
		else if (successStomp) {
			continue;
		}

		// エネミーの攻撃。
		if (enemy->GetBodyCollider()->IsHit(player->GetBodyCollider())) {
			player->SetIsAttacked(true);
			player->ComputeAttackedDirection(enemy->GetPosition());
			enemy->SetIsCoolDown(true);
		}
	}

	// プレイヤーとボスエネミー
	{

	}
}



/********************************/


CollisionManagerObject::CollisionManagerObject()
{
	m_collisionHitManager = CollisionHitManager::Create();
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