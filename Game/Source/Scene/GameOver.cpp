#include "stdafx.h"
#include "GameOver.h"
#include "Source/UI/UIGameOver.h"
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
	m_gameOverUI = NewGO<UIGameOver>(0, "UIGameOver");
	return true;
}


void GameOver::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		SceneManager::GetInstance()->ChangeScene(SceneID::Title);
	}
}