#pragma once

/// <summary>
/// シーンのインターフェース。
/// 純粋仮想関数で、派生先に実装を強制する。
/// </summary>
class IScene
{
public:
	IScene() {};
	~IScene() {};

	virtual bool Start() = 0;
	virtual void Update() = 0;
};



enum class SceneID : uint8_t
{
	Title = 0,
	InGame,
	GameClear,
	GameOver,
	None = 0xff	// 0xで16進数。ffは10進数で255。
};



/// <summary>
/// シーンの管理者。
/// シーン切り替え・更新の提供を行う。
/// </summary>
class SceneManager
{
private:
	IScene* m_currentScene = nullptr;
	SceneID m_currentID = SceneID::Title;
	SceneID m_requestID = SceneID::Title;


public:
	void Update();
	void ChangeScene(SceneID nextID)
	{
		m_requestID = nextID;
	};


private:
	SceneManager() {};
	~SceneManager() {};


private:
	static SceneManager* m_instance;
public:
	static void CreateInstance()
	{
		if (!m_instance) {
			m_instance = new SceneManager();
		}
	};
	static SceneManager* GetInstance()
	{
		return m_instance;
	};
	static void DeleteInstance()
	{
		if (m_instance) {
			delete m_instance;
			m_instance = nullptr;
		}
	};
};