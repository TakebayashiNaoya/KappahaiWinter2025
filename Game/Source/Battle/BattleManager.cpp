#include "stdafx.h"
#include "BattleManager.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/UI/UIPlayerLife.h"
#include "Source/UI/UIDamageFlash.h"
#include "Source/UI/UIBossLife.h"


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
	// シーン切り替えリクエストがある場合、バトル処理を全てスキップ。
	if (SceneManager::GetInstance()->IsSceneChangeRequested()) {
		return;
	}


	// キャラの数は少なく、FindGOs内で最適化されているため、毎フレーム取得しても問題ないと判断。
	Player* player = FindGO<Player>("Player");
	if (player == nullptr) {
		return;
	}


	// プレイヤーのライフをUIに反映。
	UIPlayerLife* playerHpUI = FindGO<UIPlayerLife>("UIPlayerLife");
	if (playerHpUI) {
		playerHpUI->SetPlayerHp(player->GetLife());
	}


	// ダメージフラッシュUIにプレイヤーのダメージ状態を反映。
	UIDamageFlash* damageFlashUI = FindGO<UIDamageFlash>("UIDamageFlash");
	if (damageFlashUI) {
		damageFlashUI->SetPlayerHp(player->GetLife());
	}


	// プレイヤーがベーシックエネミーに近づいたら、ベーシックエネミーにプレイヤーの座標を伝える。
	std::vector<BasicEnemy*> basicEnemys = FindGOs<BasicEnemy>("BasicEnemy");
	CheckEnemyDetection<BasicEnemy>(player, basicEnemys, PLAYER_SEARCH_RADIUS);


	// プレイヤーが変形エネミーに近づいたら、変形エネミーにプレイヤーの座標を伝える。
	std::vector<DeformEnemy*> transformEnemys = FindGOs<DeformEnemy>("DeformEnemy");
	CheckEnemyDetection<DeformEnemy>(player, transformEnemys, PLAYER_SEARCH_RADIUS);


	// ボスエネミーにプレイヤーの座標を伝える。
	BossEnemy* bossEnemy = FindGO<BossEnemy>("BossEnemy");
	if (bossEnemy && player) {
		bossEnemy->SetIsFoundPlayer(true, player->GetPosition());
	}


	// ボスのライフをUIに反映。
	UIBossLife* bossHpUI = FindGO<UIBossLife>("UIBossLife");
	if (bossHpUI && bossEnemy) {
		bossHpUI->SetMaxLife(bossEnemy->GetMaxLife());
		bossHpUI->SetCurrentLife(bossEnemy->GetLife());
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