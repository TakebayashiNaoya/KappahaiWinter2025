#pragma once
#include "Source/Scene/SceneManager.h"

class BackGround;
class Player;
class BasicEnemy;
class TransformEnemy;
class GameCamera;
class InGameUI;

class InGame :public IScene
{
public:
	InGame();
	~InGame();


private:
	bool Start() override final;
	void Update() override final;


private:
	BackGround* m_backGround = nullptr;
	Player* m_player = nullptr;
	BasicEnemy* m_basicEnemy = nullptr;
	TransformEnemy* m_transformEnemy = nullptr;
	GameCamera* m_gameCamera = nullptr;
	InGameUI* m_inGameUI = nullptr;


private:
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;		//スカイキューブ。
	int m_skyCubeType = enSkyCubeType_Night;
};

