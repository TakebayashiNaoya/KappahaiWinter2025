#pragma once
#include "Source/Scene/BattleStageBase.h"

class FirstPlanet;
class Treasure;
class Rocket;
class UIGear;


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
	std::vector<Treasure*> m_treasures;
	Rocket* m_rocket = nullptr;
	UIGear* m_uiGear = nullptr;
};

