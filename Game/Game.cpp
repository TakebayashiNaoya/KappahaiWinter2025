#include "stdafx.h"
#include "Game.h"
#include "Source/Scene/SceneManager.h"


Game::Game()
{
	m_sceneManager = NewGO<SceneManagerObject>(0, "SceneManagerObject");
}


Game::~Game()
{
	DeleteGO(m_sceneManager);
}


bool Game::Start()
{
	return true;
}


void Game::Update()
{
}


void Game::Render(RenderContext& rc)
{
}