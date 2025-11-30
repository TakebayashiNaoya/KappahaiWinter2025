#pragma once

class LoadingScreen
{
public:
	enum EnState
	{
		enState_Close,
		enState_Loading,
		enState_Open,
		enState_Opened,
		enState_None
	};

	// ▼▼▼ 実装を .cpp へ移動するため、宣言のみに変更 ▼▼▼
	static void StartLoading();
	static void FinishLoading();
	static void ChangeState(EnState state);
	static const EnState GetState();
	// ▲▲▲ 変更ここまで ▲▲▲

private:
	enum EnImageParts
	{
		enImageParts_Center,
		enImageParts_Top,
		enImageParts_Bottom,
		enImageParts_Left,
		enImageParts_Right,
		enImageParts_Num
	};

private:
	LoadingScreen();
	~LoadingScreen();

	void ForceToLoadingLayout();

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender* m_displayImages = nullptr;
	SpriteRender m_loadingIcon;
	EnState m_state = enState_None;
	float m_timer = 0.0f;

private:
	void InitLayout();

	/**
	 * シングルトン関連
	 */
private:
	static LoadingScreen* m_instance;

public:
	static LoadingScreen* CreateInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new LoadingScreen();
		}
		return m_instance;
	}
	static LoadingScreen* GetInstance()
	{
		return m_instance;
	}
	static bool IsAvailable()
	{
		return m_instance != nullptr;
	}
	static void Delete()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
};

class LoadingScreenObject : public IGameObject
{
public:
	LoadingScreenObject();
	~LoadingScreenObject();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
};