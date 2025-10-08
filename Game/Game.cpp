#include "stdafx.h"
#include "Game.h"
#include "Source/Actor/Stage/BackGround.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Camera/GameCamera.h"

bool Game::Start()
{
	m_backGround = NewGO<FirstPlanet>(0, "BackGround");
	m_player = NewGO<Player>(0, "Player");
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	return true;
}

void Game::Update()
{
}

void Game::Render(RenderContext& rc)
{
}