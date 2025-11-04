#include "stdafx.h"
#include "GameOver.h"
#include "Source/UI/GameOverUI.h"
#include "Source/Scene/SceneManager.h"


GameOver::GameOver()
{

}


GameOver::~GameOver()
{
	DeleteGO(m_gameOverUI);
}


bool GameOver::Start()
{
	m_gameOverUI = NewGO<GameOverUI>(0, "GameOverUI");
	return true;
}


void GameOver::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		SceneManager::GetInstance()->ChangeScene(SceneID::Title);
	}
}