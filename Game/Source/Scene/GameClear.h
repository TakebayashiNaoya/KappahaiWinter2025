#pragma once
#include "Source/Scene/SceneManager.h"


class GameClear :public IScene
{
public:
	GameClear();
	~GameClear();

private:
	bool Start() override final;
	void Update() override final;
};

