#pragma once


class Player;
class BossEnemy;
class BasicEnemy;
class DeformEnemy;
class UIPlayerLife;
class UIDamageFlash;
class UIBossLife;
class UIGear;
class Rocket;
class Treasure;


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
	/// フラグの取得・設定
	/// </summary>
public:
	static bool GetIsBattleFinish()
	{
		return m_isBattleFinish;
	}
	static void SetIsBattleFinish(bool isFinish)
	{
		m_isBattleFinish = isFinish;
	}
	static bool GetIsStopCollisionManager()
	{
		return m_isStopCollisionManager;
	}
	static void SetIsStopCollisionManager(bool isStop)
	{
		m_isStopCollisionManager = isStop;
	}


	/// <summary>
	/// 登録・解除関数群
	/// </summary>
public:
	// エネミー全削除用
	void DestroyAllEnemies();

	// プレイヤー用
	void Register(Player* player);
	void Unregister(Player* player);

	// ボス用
	void Register(BossEnemy* boss);
	void Unregister(BossEnemy* boss);

	// 基本エネミー用
	void Register(BasicEnemy* enemy);
	void Unregister(BasicEnemy* enemy);

	// 変形エネミー用
	void Register(DeformEnemy* enemy);
	void Unregister(DeformEnemy* enemy);

	// プレイヤーライフUI用
	void Register(UIPlayerLife* uiPlayerLife);
	void Unregister(UIPlayerLife* uiPlayerLife);

	// ダメージフラッシュUI用
	void Register(UIDamageFlash* uiDamageFlash);
	void Unregister(UIDamageFlash* uiDamageFlash);

	// ボスライフUI用
	void Register(UIBossLife* uiBossLife);
	void Unregister(UIBossLife* uiBossLife);

	// ギアUI用
	void Register(UIGear* uiGear);
	void Unregister(UIGear* uiGear);

	// ロケット用
	void Register(Rocket* rocket);
	void Unregister(Rocket* rocket);

	// 宝箱用
	void Register(Treasure* treasure);
	void Unregister(Treasure* treasure);

	// その他汎用
	template<typename T>
	void Register(T* object)
	{
		K2_ASSERT(false, "しょりついかわすれ");
	}
	template<typename T>
	void Unregister(T* object)
	{
		K2_ASSERT(false, "しょりついかわすれ");
	}


private:
	static bool m_isBattleFinish;
	static bool m_isStopCollisionManager;

	Player* m_player = nullptr;
	BossEnemy* m_bossEnemy = nullptr;
	std::vector<BasicEnemy*> m_basicEnemies;
	std::vector<DeformEnemy*> m_deformEnemies;

	UIPlayerLife* m_uiPlayerLife = nullptr;
	UIDamageFlash* m_uiDamageFlash = nullptr;
	UIBossLife* m_uiBossLife = nullptr;
	UIGear* m_uiGear = nullptr;
	int m_gotGearCount = 0;
	int m_maxGearCount = 0;
	bool m_canLaunch = false;

	Rocket* m_rocket = nullptr;
	std::vector<Treasure*> m_treasures;

	std::vector<IGameObject*> m_objects;
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

