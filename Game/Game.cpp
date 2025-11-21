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
	//auto* pointLight = g_sceneLight->NewPointLight();
	//pointLight->SetPosition(Vector3(5.0f, 2000.0f, 0.0f));
	//pointLight->SetColor(Vector3(1.0f, 0.1f, 100.0f));
	//pointLight->SetRange(100.0f);
	//pointLight->SetAffectPowParam(1.0f);
	//pointLight->Update();

	//pointLight = g_sceneLight->NewPointLight();
	//pointLight->SetPosition(Vector3(50.0f, 0.0f, 2276.0f));
	//pointLight->SetColor(Vector3(1.0f, 1000.1f, 10.0f));
	//pointLight->SetRange(1000.0f);
	//pointLight->SetAffectPowParam(1.0f);
	//pointLight->Update();

	//pointLight = g_sceneLight->NewPointLight();
	//pointLight->SetPosition(Vector3(5.0f, 0.0f, 2276.0f));
	//pointLight->SetColor(Vector3(100.0f, 0.1f, 100.0f));
	//pointLight->SetRange(10000.0f);
	//pointLight->SetAffectPowParam(1.0f);
	//pointLight->Update();

	//pointLight = g_sceneLight->NewPointLight();
	//pointLight->SetPosition(Vector3(-105.0f, 0.0f, 2276.0f));
	//pointLight->SetColor(Vector3(100.0f, 0.1f, 1.0f));
	//pointLight->SetRange(10000.0f);
	//pointLight->SetAffectPowParam(1.0f);
	//pointLight->Update();

	return true;
}


void Game::Update()
{
}


void Game::Render(RenderContext& rc)
{
}