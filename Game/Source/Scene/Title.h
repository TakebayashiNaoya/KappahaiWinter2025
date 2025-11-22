#pragma once
#include "Source/Scene/SceneManager.h"


class UITitle;
class TitlePlayer;
class TitleCamera;
class TitleStage;
class UITitle;


class Title :public IScene
{
public:
	Title();
	~Title();


private:
	bool Start() override final;
	void Update() override final;


private:
	UITitle* m_titleUI = nullptr;
	TitlePlayer* m_titlePlayer = nullptr;
	TitleCamera* m_titleCamera = nullptr;
	TitleStage* m_titleStage = nullptr;
	UITitle* m_uiTitle = nullptr;


private:
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;		//スカイキューブ。
	int m_skyCubeType = enSkyCubeType_SpaceToon_2;
};