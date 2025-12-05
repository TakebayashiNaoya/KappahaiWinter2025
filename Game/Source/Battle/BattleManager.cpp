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
#include "Source/Actor/Object/Treasure/Treasure.h"


namespace
{
	constexpr float ENEMY_SEARCH_RADIUS = 500.0f;	// プレイヤー検出半径
	constexpr float ROCKET_SEARCH_RADIUS = 500.0f;	// ロケットのプレイヤー検出半径
	constexpr float TREASURE_SEARCH_RADIUS = 200.0f;	// 宝箱のプレイヤー検出半径

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

	/// <summary>
	/// 死亡しているエネミーをDeleteGOし、リストからも削除するテンプレート関数
	/// </summary>
	template <class T>
	void UpdateAndRemoveDeadEnemies(std::vector<T*>& enemies)
	{
		auto it = enemies.begin();
		while (it != enemies.end()) {
			auto* enemy = *it;
			// エネミーが存在し、かつ死んでいる場合
			if (enemy && enemy->IsDying()) {
				DeleteGO(enemy);        // メモリ削除予約
				it = enemies.erase(it); // リストから削除して、イテレータを進める
			}
			else {
				++it; // 死んでいなければ次の要素へ
			}
		}
	}
}


BattleManager* BattleManager::m_instance = nullptr;
bool BattleManager::m_isBattleFinish = false;
bool BattleManager::m_isStopCollisionManager = false;


void BattleManager::Update()
{
	// シーン切り替えリクエストがある場合、バトル処理を全てスキップ。
	if (SceneManager::GetInstance()->GetIsSceneChangeRequested()) {
		return;
	}

	// 死亡しているエネミーをDeleteGOし、リストからも削除。
	UpdateAndRemoveDeadEnemies(m_basicEnemies);
	UpdateAndRemoveDeadEnemies(m_deformEnemies);

	// ボスエネミーにプレイヤーの座標を伝える。
	if (m_bossEnemy && m_player) {
		m_bossEnemy->SetIsFoundPlayer(true, m_player->GetPosition());
	}

	// プレイヤーがベーシックエネミーに近づいたら、ベーシックエネミーにプレイヤーの座標を伝える。
	CheckEnemyDetection<BasicEnemy>(m_player, m_basicEnemies, ENEMY_SEARCH_RADIUS);

	// プレイヤーが変形エネミーに近づいたら、変形エネミーにプレイヤーの座標を伝える。
	CheckEnemyDetection<DeformEnemy>(m_player, m_deformEnemies, ENEMY_SEARCH_RADIUS);

	// プレイヤーのライフをUIに反映。
	if (m_uiPlayerLife && m_player) {
		m_uiPlayerLife->SetPlayerHp(m_player->GetLife());
	}

	// ダメージフラッシュUIにプレイヤーのダメージ状態を反映。
	if (m_uiDamageFlash && m_player) {
		m_uiDamageFlash->SetPlayerHp(m_player->GetLife());
	}

	// ボスのライフをUIに反映。
	if (m_uiBossLife && m_bossEnemy) {
		m_uiBossLife->SetMaxLife(m_bossEnemy->GetMaxLife());
		m_uiBossLife->SetCurrentLife(m_bossEnemy->GetLife());
	}

	// プレイヤーがロケットに近づいたら、ロケットをゴール状態にする。
	if (m_rocket && m_player) {
		Vector3 lengthVec = m_rocket->GetPosition() - m_player->GetPosition();
		if (lengthVec.Length() < ROCKET_SEARCH_RADIUS) {
			if (m_rocket != nullptr) {
				m_rocket->SetIsGooled(true);
			}
		}
	}

	// プレイヤーが宝箱に近づいたら、宝箱を開ける。
	if (m_player) {
		for (auto* treasure : m_treasures) {
			if (treasure == nullptr) {
				continue;
			}
			Vector3 lengthVec = treasure->GetPosition() - m_player->GetPosition();
			if (lengthVec.Length() < TREASURE_SEARCH_RADIUS) {
				treasure->SetIsOpened(true);
			}
		}
	}
}

void BattleManager::DestroyAllEnemies()
{
	// 基本エネミーの削除
	for (auto* enemy : m_basicEnemies) {
		if (enemy) {
			DeleteGO(enemy);
		}
	}
	m_basicEnemies.clear();

	// 変形エネミーの削除
	for (auto* enemy : m_deformEnemies) {
		if (enemy) {
			DeleteGO(enemy);
		}
	}
	m_deformEnemies.clear();

	// ※ボスなどはステージ個別の管理でよければそのままでもOKですが、
	// ここでまとめて消す設計にしても構いません。
}

void BattleManager::Register(Player* player)
{
	m_player = player;
}

void BattleManager::Unregister(Player* player)
{
	m_player = nullptr;
}

void BattleManager::Register(BossEnemy* boss)
{
	m_bossEnemy = boss;
}

void BattleManager::Unregister(BossEnemy* boss)
{
	m_bossEnemy = nullptr;
}

void BattleManager::Register(BasicEnemy* enemy)
{
	m_basicEnemies.push_back(enemy);
}

void BattleManager::Unregister(BasicEnemy* enemy)
{
	auto it = std::remove(m_basicEnemies.begin(), m_basicEnemies.end(), enemy);
	m_basicEnemies.erase(it, m_basicEnemies.end());
}

void BattleManager::Register(DeformEnemy* enemy)
{
	m_deformEnemies.push_back(enemy);
}

void BattleManager::Unregister(DeformEnemy* enemy)
{
	auto it = std::remove(m_deformEnemies.begin(), m_deformEnemies.end(), enemy);
	m_deformEnemies.erase(it, m_deformEnemies.end());
}

void BattleManager::Register(UIPlayerLife* uiPlayerLife)
{
	m_uiPlayerLife = uiPlayerLife;
}

void BattleManager::Unregister(UIPlayerLife* uiPlayerLife)
{
	m_uiPlayerLife = nullptr;
}

void BattleManager::Register(UIDamageFlash* uiDamageFlash)
{
	m_uiDamageFlash = uiDamageFlash;
}

void BattleManager::Unregister(UIDamageFlash* uiDamageFlash)
{
	m_uiDamageFlash = nullptr;
}

void BattleManager::Register(UIBossLife* uiBossLife)
{
	m_uiBossLife = uiBossLife;
}

void BattleManager::Unregister(UIBossLife* uiBossLife)
{
	m_uiBossLife = nullptr;
}

void BattleManager::Register(Rocket* rocket)
{
	m_rocket = rocket;
}

void BattleManager::Unregister(Rocket* rocket)
{
	m_rocket = nullptr;
}

void BattleManager::Register(Treasure* treasure)
{
	m_treasures.push_back(treasure);
}

void BattleManager::Unregister(Treasure* treasure)
{
	auto it = std::remove(m_treasures.begin(), m_treasures.end(), treasure);
	m_treasures.erase(it, m_treasures.end());
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