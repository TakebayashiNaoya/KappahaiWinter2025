#include "stdafx.h"
#include "Game.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"


Game::Game()
{
	m_sceneManager = NewGO<SceneManagerObject>(0, "SceneManagerObject");
	m_battleManager = NewGO<BattleManagerObject>(0, "BattleManagerObject");
	m_collisionManager = NewGO<CollisionManagerObject>(0, "CollisionManagerObject");
}


Game::~Game()
{
	DeleteGO(m_sceneManager);
	DeleteGO(m_battleManager);
	DeleteGO(m_collisionManager);
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