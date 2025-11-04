#include "stdafx.h"
#include "GameOverUI.h"

GameOverUI::GameOverUI()
{
	m_gameOverImage.Init("Assets/sprite/GameOver.dds", 1920.0f, 1080.0f);
}

GameOverUI::~GameOverUI()
{
}

bool GameOverUI::Start()
{
	return true;
}

void GameOverUI::Update()
{
}

void GameOverUI::Render(RenderContext& rc)
{
	//if (m_isDrawStop) {
	//	return;
	//}
	m_gameOverImage.Draw(rc);
}
