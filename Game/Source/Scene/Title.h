#pragma once
#include "Source/Scene/SceneManager.h"


class TitleUI;


class Title :public IScene
{
public:
	Title();
	~Title();


private:
	bool Start() override final;
	void Update() override final;


private:
	TitleUI* m_titleUI = nullptr;
};