#pragma once

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

