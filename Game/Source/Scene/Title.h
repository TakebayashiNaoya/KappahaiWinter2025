#pragma once
#include "Source/Scene/SceneManager.h"


class UITitle;


class Title :public IScene
{
public:
	Title();
	~Title();


private:
	bool Start() override final;
	void Update() override final;


private:
	UITitle* m_titleUI = nullptr;
};