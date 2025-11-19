#pragma once
#include "Source/Scene/SceneManager.h"

class UIGameOver;

class GameOver :public IScene
{
public:
	GameOver();
	~GameOver();


private:
	bool Start() override final;
	void Update() override final;


private:
	UIGameOver* m_gameOverUI = nullptr;
};

