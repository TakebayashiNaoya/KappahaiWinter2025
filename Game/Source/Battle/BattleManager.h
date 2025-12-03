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
	void RegisterPlayer(Player* player);
	void UnregisterPlayer();

	// ボス用
	void RegisterBoss(BossEnemy* boss);
	void UnregisterBoss();

	// 基本エネミー用
	void RegisterBasicEnemy(BasicEnemy* enemy);
	void UnregisterBasicEnemy(BasicEnemy* enemy);

	// 変形エネミー用
	void RegisterDeformEnemy(DeformEnemy* enemy);
	void UnregisterDeformEnemy(DeformEnemy* enemy);

	// ロケット用
	void RegisterRocket(Rocket* rocket);
	void UnregisterRocket();

	// プレイヤーライフUI用
	void RegisterUIPlayerLife(UIPlayerLife* uiPlayerLife);
	void UnregisterUIPlayerLife();

	// ダメージフラッシュUI用
	void RegisterUIDamageFlash(UIDamageFlash* uiDamageFlash);
	void UnregisterUIDamageFlash();

	// ボスライフUI用
	void RegisterUIBossLife(UIBossLife* uiBossLife);
	void UnregisterUIBossLife();


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

