#include "stdafx.h"
#include "BattleManager.h"

#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"


namespace
{
	constexpr float PLAYER_SEARCH_RADIUS = 500.0f;	// プレイヤー検出半径
}

BattleManager* BattleManager::m_instance = nullptr;


void BattleManager::Update()
{
	// キャラの数は少なく、FindGOs内で最適化されているため、毎フレーム取得しても問題ないと判断。
	Player* player = FindGO<Player>("Player");
	std::vector<BasicEnemy*> basicEnemys = FindGOs<BasicEnemy>("BasicEnemy");
	std::vector<TransformEnemy*> transformEnemys = FindGOs<TransformEnemy>("TransformEnemy");

	// プレイヤーがベーシックエネミーに近づいたら、ベーシックエネミーにプレイヤーの座標を伝える。
	for (auto* enemy : basicEnemys) {
		Vector3 distance = enemy->GetPosition() - player->GetPosition();
		if (distance.Length() < PLAYER_SEARCH_RADIUS) {
			enemy->SetIsFoundPlayer(true, player->GetPosition());
		}
		else {
			enemy->SetIsFoundPlayer(false, Vector3::Zero);
		}
	}

	// プレイヤーが変形エネミーに近づいたら、変形エネミーにプレイヤーの座標を伝える。
	for (auto* enemy : transformEnemys) {
		Vector3 distance = enemy->GetPosition() - player->GetPosition();
		if (distance.Length() < PLAYER_SEARCH_RADIUS) {
			enemy->SetIsFoundPlayer(true, player->GetPosition());
		}
		else {
			enemy->SetIsFoundPlayer(false, Vector3::Zero);
		}
	}
}



/********************************/


BattleManagerObject::BattleManagerObject()
{
	m_battleManager = BattleManager::Create();
}


BattleManagerObject::~BattleManagerObject()
{
	BattleManager::Delete();
	m_battleManager = nullptr;
}


bool BattleManagerObject::Start()
{
	return true;
}


void BattleManagerObject::Update()
{
	m_battleManager->Update();
}