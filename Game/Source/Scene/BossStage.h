#pragma once
#include "SceneManager.h"

class BossPlanet;
class Player;
class Spawner;
class BossEnemy;
class GameCamera;
class UIInGame;
class BattleManagerObject;
class CollisionManagerObject;
class UIBossLife;

class BossStage : public IScene
{
public:
	BossStage();
	~BossStage();


private:
	bool Start() override final;
	void Update() override final;

	void InitObjects();
	void InitLevel();
	LevelRender m_levelRender;		//レベルレンダー。


private:
	BossPlanet* m_bossPlanet = nullptr;
	Player* m_player = nullptr;
	std::vector<Spawner*> m_spawners;
	BossEnemy* m_bossEnemy = nullptr;
	GameCamera* m_gameCamera = nullptr;
	UIInGame* m_inGameUI = nullptr;
	BattleManagerObject* m_battleManager = nullptr;
	CollisionManagerObject* m_collisionManager = nullptr;
	UIBossLife* m_uiBossLife = nullptr;


private:
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();

	SkyCube* m_skyCube = nullptr;		//スカイキューブ。
	int m_skyCubeType = enSkyCubeType_Night;
	int m_loadingState = 0;
};