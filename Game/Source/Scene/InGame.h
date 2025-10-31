#pragma once
#include "Source/Scene/SceneManager.h"
class InGame :public IScene
{
public:
	InGame();
	~InGame();

private:
	bool Start() override final;
	void Update() override final;
};

