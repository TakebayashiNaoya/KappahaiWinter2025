#include "stdafx.h"
#include "Game.h"
#include "Source/Actor/Stage/BackGround.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Camera/GameCamera.h"

bool Game::Start()
{
	InitSky();
	m_backGround = NewGO<FirstPlanet>(0, "BackGround");
	m_player = NewGO<Player>(0, "Player");
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	return true;
}

void Game::Update()
{
	Vector3 pos = m_player->GetPosition();


	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d,%d,%d", int(pos.x), int(pos.y), int(pos.z));
	currentPos.SetText(wcsbuf);
	currentPos.SetPosition(Vector3(0.0f, 400.0f, 0.0f));
	currentPos.SetScale(1.0f);
	currentPos.SetColor(g_vec4White);
}

void Game::Render(RenderContext& rc)
{
	currentPos.Draw(rc);
}

void Game::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}