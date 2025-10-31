#pragma once
#include "Source/Scene/SceneManager.h"

class Title :public IScene
{
public:
	Title();
	~Title();

private:
	bool Start() override final;
	void Update() override final;
};