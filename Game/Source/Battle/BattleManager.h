#pragma once


class Player;
class BasicEnemy;
class DeformEnemy;
class BossEnemy;
class Rocket;
class UIPlayerLife;
class UIDamageFlash;
class UIBossLife;


// 当たり判定を管理するクラス
// 当たったという処理をまとめたい
class BattleManager
{
private:
	BattleManager() {};
	~BattleManager() {};


public:
	void Update();


private:
	static BattleManager* m_instance;


public:
	static BattleManager* Create()
	{
		if (m_instance == nullptr) {
			m_instance = new BattleManager();
		}
		return m_instance;
	}
	static void Delete()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
	static BattleManager* GetInstance()
	{
		return m_instance;
	}


	/// <summary>
	/// バトル終了フラグの取得・設定
	/// </summary>
public:
	static bool IsBattleFinish()
	{
		return m_isBattleFinish;
	}
	static void SetIsBattleFinish(bool isFinish)
	{
		m_isBattleFinish = isFinish;
	}


	/// <summary>
	/// 登録・解除関数群
	/// </summary>
public:
	// プレイヤー用
	void RegisterPlayer(Player* player)
	{
		m_player = player;
	}
	void UnregisterPlayer()
	{
		m_player = nullptr;
	}

	// ボス用
	void RegisterBoss(BossEnemy* boss)
	{
		m_bossEnemy = boss;
	}
	void UnregisterBoss()
	{
		m_bossEnemy = nullptr;
	}

	// 基本エネミー用
	void RegisterBasicEnemy(BasicEnemy* enemy)
	{
		m_basicEnemies.push_back(enemy);
	}
	void UnregisterBasicEnemy(BasicEnemy* enemy)
	{
		auto it = std::remove(m_basicEnemies.begin(), m_basicEnemies.end(), enemy);
		m_basicEnemies.erase(it, m_basicEnemies.end());
	}

	// 変形エネミー用
	void RegisterDeformEnemy(DeformEnemy* enemy)
	{
		m_deformEnemies.push_back(enemy);
	}
	void UnregisterDeformEnemy(DeformEnemy* enemy)
	{
		auto it = std::remove(m_deformEnemies.begin(), m_deformEnemies.end(), enemy);
		m_deformEnemies.erase(it, m_deformEnemies.end());
	}

	// ロケット用
	void RegisterRocket(Rocket* rocket)
	{
		m_rocket = rocket;
	}
	void UnregisterRocket()
	{
		m_rocket = nullptr;
	}

	// プレイヤーライフUI用
	void RegisterUIPlayerLife(UIPlayerLife* uiPlayerLife)
	{
		m_uiPlayerLife = uiPlayerLife;
	}
	void UnregisterUIPlayerLife()
	{
		m_uiPlayerLife = nullptr;
	}

	// ダメージフラッシュUI用
	void RegisterUIDamageFlash(UIDamageFlash* uiDamageFlash)
	{
		m_uiDamageFlash = uiDamageFlash;
	}
	void UnregisterUIDamageFlash()
	{
		m_uiDamageFlash = nullptr;
	}

	// ボスライフUI用
	void RegisterUIBossLife(UIBossLife* uiBossLife)
	{
		m_uiBossLife = uiBossLife;
	}
	void UnregisterUIBossLife()
	{
		m_uiBossLife = nullptr;
	}


private:
	static bool m_isBattleFinish;

	Player* m_player = nullptr;
	BossEnemy* m_bossEnemy = nullptr;
	std::vector<BasicEnemy*> m_basicEnemies;
	std::vector<DeformEnemy*> m_deformEnemies;

	Rocket* m_rocket = nullptr;
	UIPlayerLife* m_uiPlayerLife = nullptr;
	UIDamageFlash* m_uiDamageFlash = nullptr;
	UIBossLife* m_uiBossLife = nullptr;
};



/// <summary>
/// 当たり判定管理クラスを更新したりするためのゲームオブジェクト。
/// </summary>
class BattleManagerObject : public IGameObject
{
public:
	BattleManagerObject();
	~BattleManagerObject();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& renderContext) override {}	// Renderはない


private:
	BattleManager* m_battleManager = nullptr;
};

