#pragma once


// 当たり判定を管理するクラス
// 当たったという処理をまとめたい
class CollisionHitManager
{
private:
	CollisionHitManager() {};
	~CollisionHitManager() {};


public:
	void Update();



private:
	static CollisionHitManager* m_instance;


public:
	static CollisionHitManager* Create()
	{
		if (m_instance == nullptr) {
			m_instance = new CollisionHitManager();
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
class CollisionManagerObject : public IGameObject
{
public:
	CollisionManagerObject();
	~CollisionManagerObject();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& renderContext) override {}	// Renderはない


private:
	CollisionHitManager* m_collisionHitManager = nullptr;
};

