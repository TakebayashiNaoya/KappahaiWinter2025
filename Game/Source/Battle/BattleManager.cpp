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
#include "Source/Actor/Object/Rocket/Rocket.h"


namespace
{
	constexpr float ENEMY_SEARCH_RADIUS = 500.0f;	// プレイヤー検出半径
	constexpr float ROCKET_SEARCH_RADIUS = 800.0f;	// ロケットのプレイヤー検出半径

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
bool BattleManager::m_isBattleFinish = false;


void BattleManager::Update()
{
	// シーン切り替えリクエストがある場合、バトル処理を全てスキップ。
	if (SceneManager::GetInstance()->IsSceneChangeRequested()) {
		return;
	}


	// キャラの数は少なく、FindGOs内で最適化されているため、毎フレーム取得しても問題ないと判断。
	//Player* player = FindGO<Player>("Player");
	if (m_player == nullptr) {
		return;
	}


	// プレイヤーのライフをUIに反映。
	//UIPlayerLife* playerHpUI = FindGO<UIPlayerLife>("UIPlayerLife");
	if (m_uiPlayerLife && m_player) {
		m_uiPlayerLife->SetPlayerHp(m_player->GetLife());
	}


	// ダメージフラッシュUIにプレイヤーのダメージ状態を反映。
	if (m_uiDamageFlash && m_player) {
		m_uiDamageFlash->SetPlayerHp(m_player->GetLife());
	}


	//Rocket* rocket = FindGO<Rocket>("Rocket");
	if (m_rocket && m_player) {
		Vector3 lengthVec = m_rocket->GetPosition() - m_player->GetPosition();
		if (lengthVec.Length() < ROCKET_SEARCH_RADIUS) {
			m_rocket->SetIsGooled(true);
		}
	}


	// プレイヤーがベーシックエネミーに近づいたら、ベーシックエネミーにプレイヤーの座標を伝える。
	//std::vector<BasicEnemy*> basicEnemys = FindGOs<BasicEnemy>("BasicEnemy");
	CheckEnemyDetection<BasicEnemy>(m_player, m_basicEnemies, ENEMY_SEARCH_RADIUS);


	// プレイヤーが変形エネミーに近づいたら、変形エネミーにプレイヤーの座標を伝える。
	//std::vector<DeformEnemy*> transformEnemys = FindGOs<DeformEnemy>("DeformEnemy");
	CheckEnemyDetection<DeformEnemy>(m_player, m_deformEnemies, ENEMY_SEARCH_RADIUS);


	// ボスエネミーにプレイヤーの座標を伝える。
	//BossEnemy* bossEnemy = FindGO<BossEnemy>("BossEnemy");
	if (m_bossEnemy && m_player) {
		m_bossEnemy->SetIsFoundPlayer(true, m_player->GetPosition());
	}


	// ボスのライフをUIに反映。
	if (m_uiBossLife && m_bossEnemy) {
		m_uiBossLife->SetMaxLife(m_bossEnemy->GetMaxLife());
		m_uiBossLife->SetCurrentLife(m_bossEnemy->GetLife());
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