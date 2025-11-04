#include "stdafx.h"
#include "BattleManager.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"
#include "Source/UI/InGameUI.h"


namespace
{
	constexpr float PLAYER_SEARCH_RADIUS = 500.0f;	// プレイヤー検出半径

	/// <summary>
	/// プレイヤーがエネミーに近づいたら、エネミーにプレイヤーの座標を伝え、発見フラグを立てる。
	/// </summary>
	/// <typeparam name="T"> 敵オブジェクトの型（vector）。</typeparam>
	/// <param name="player"> プレイヤーのポインタ。</param>
	/// <param name="enemys"> エネミーのvector型の変数。</param>
	/// <param name="searchRadius">敵を検出するための半径。</param>
	template <class T>
	void CheckEnemyDetection(Player* player, std::vector<T*>& enemys, float searchRadius)
	{
		if (player == nullptr) {
			return;
		}

		for (auto* enemy : enemys) {

			if (enemy == nullptr) {
				continue;
			}

			Vector3 distance = enemy->GetPosition() - player->GetPosition();
			if (distance.Length() < searchRadius) {
				enemy->SetIsFoundPlayer(true, player->GetPosition());
			}
			else {
				enemy->SetIsFoundPlayer(false, Vector3::Zero);
			}
		}
	}
}

BattleManager* BattleManager::m_instance = nullptr;




void BattleManager::Update()
{
	//// シーン切り替えリクエストがある場合、バトル処理を全てスキップ。
	//if (SceneManager::GetInstance()->IsSceneChangeRequested()) {
	//	return;
	//}



	// キャラの数は少なく、FindGOs内で最適化されているため、毎フレーム取得しても問題ないと判断。
	Player* player = FindGO<Player>("Player");

	// プレイヤーがベーシックエネミーに近づいたら、ベーシックエネミーにプレイヤーの座標を伝える。
	std::vector<BasicEnemy*> basicEnemys = FindGOs<BasicEnemy>("BasicEnemy");
	CheckEnemyDetection<BasicEnemy>(player, basicEnemys, PLAYER_SEARCH_RADIUS);

	// プレイヤーが変形エネミーに近づいたら、変形エネミーにプレイヤーの座標を伝える。
	std::vector<TransformEnemy*> transformEnemys = FindGOs<TransformEnemy>("TransformEnemy");
	CheckEnemyDetection<TransformEnemy>(player, transformEnemys, PLAYER_SEARCH_RADIUS);

	// インゲームUIにライフを反映。
	InGameUI* inGameUI = FindGO<InGameUI>("InGameUI");
	if (inGameUI) {
		if (player) {
			inGameUI->SetLife(player->GetLife());
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