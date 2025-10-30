#include "stdafx.h"
#include "Game.h"
#include "Source/Actor/Stage/BackGround.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/Collision/CollisionManager.h"
#include "Source/Battle/BattleManager.h"
#include "GameOver.h"

Game::~Game()
{
	DeleteGO(m_skyCube);
	DeleteGO(m_backGround);
	DeleteGO(m_player);
	DeleteGO(m_basicEnemy);
	DeleteGO(m_transformEnemy);
	DeleteGO(m_gameCamera);
	DeleteGO(m_collisionManager);
	DeleteGO(m_battleManager);
}

bool Game::Start()
{
	InitSky();
	m_backGround = NewGO<FirstPlanet>(0, "BackGround");
	m_player = NewGO<Player>(0, "Player");
	m_basicEnemy = NewGO<BasicEnemy>(0, "BasicEnemy");
	m_transformEnemy = NewGO<TransformEnemy>(0, "TransformEnemy");
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	m_collisionManager = NewGO<CollisionManagerObject>(0, "CollisionManagerObject");
	m_battleManager = NewGO<BattleManagerObject>(0, "BattleManagerObject");

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

	if (m_player->GetLife() == 0)
	{
		DeleteGO(this);
		NewGO<GameOver>(0, "GameOver");
	}
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