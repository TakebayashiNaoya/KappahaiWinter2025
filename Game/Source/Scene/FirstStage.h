#pragma once
#include "Source/Scene/SceneManager.h"

class FirstPlanet;
class Player;
class BasicEnemy;
class DeformEnemy;
class BossEnemy;
class GameCamera;
class UIInGame;
class BattleManagerObject;
class CollisionManagerObject;
class Rocket;


class FirstStage :public IScene
{
public:
	FirstStage();
	~FirstStage();


private:
	bool Start() override final;
	void Update() override final;

	void InitObjects();
	void InitLevel();
	LevelRender m_levelRender;		//レベルレンダー。


private:
	FirstPlanet* m_firstStage = nullptr;
	Player* m_player = nullptr;
	std::vector<BasicEnemy*> m_basicEnemies;
	std::vector<DeformEnemy*> m_deformEnemies;	// 変形エネミーの配列。
	BossEnemy* m_bossEnemy = nullptr;
	GameCamera* m_gameCamera = nullptr;
	UIInGame* m_inGameUI = nullptr;
	BattleManagerObject* m_battleManager = nullptr;
	CollisionManagerObject* m_collisionManager = nullptr;
	Rocket* m_rocket = nullptr;


private:
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;		//スカイキューブ。
	int m_skyCubeType = enSkyCubeType_Night;
	int m_loadingState = 0;
};

