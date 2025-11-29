#pragma once
#include "Source/Scene/BattleStageBase.h"

class BossPlanet;
class Spawner;
class UIBossLife;


class BossStage : public BattleStageBase
{
public:
	BossStage();
	~BossStage();


private:
	bool Start() override final;

	void OnUpdate() override final;

	void InitLevel()override final;
	LevelRender m_levelRender;		//レベルレンダー。


private:
	BossPlanet* m_bossPlanet = nullptr;
	std::vector<Spawner*> m_spawners;
	UIBossLife* m_uiBossLife = nullptr;
};