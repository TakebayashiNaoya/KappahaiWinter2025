#pragma once
#include "Source/Scene/SceneManager.h"

class FirstStage;
class Player;
class BasicEnemy;
class DeformEnemy;
class BossEnemy;
class GameCamera;
class UIInGame;
class BattleManagerObject;
class CollisionManagerObject;

class InGame :public IScene
{
public:
	InGame();
	~InGame();


private:
	bool Start() override final;
	void Update() override final;


private:
	FirstStage* m_firstStage = nullptr;
	Player* m_player = nullptr;
	BasicEnemy* m_basicEnemy = nullptr;
	DeformEnemy* m_deformEnemy = nullptr;
	BossEnemy* m_bossEnemy = nullptr;
	GameCamera* m_gameCamera = nullptr;
	UIInGame* m_inGameUI = nullptr;
	BattleManagerObject* m_battleManager = nullptr;
	CollisionManagerObject* m_collisionManager = nullptr;


private:
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;		//スカイキューブ。
	int m_skyCubeType = enSkyCubeType_Night;
};

