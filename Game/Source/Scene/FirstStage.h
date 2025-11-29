#pragma once
#include "Source/Scene/BattleStageBase.h"

class FirstPlanet;
class BasicEnemy;
class DeformEnemy;
class Rocket;


class FirstStage :public BattleStageBase
{
public:
	FirstStage();
	~FirstStage();


private:
	bool Start() override final;

	void OnUpdate() override final;

	void InitLevel()override final;
	LevelRender m_levelRender;		//レベルレンダー。


private:
	FirstPlanet* m_firstStage = nullptr;
	std::vector<BasicEnemy*> m_basicEnemies;
	std::vector<DeformEnemy*> m_deformEnemies;
	Rocket* m_rocket = nullptr;
};

