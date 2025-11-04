#include "stdafx.h"
#include "InGame.h"
#include "Source/Actor/Stage/BackGround.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/TransformEnemy/TransformEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/Scene/GameOver.h"
#include "Source/UI/InGameUI.h"


InGame::InGame()
{

}


InGame::~InGame()
{
	DeleteGO(m_skyCube);
	DeleteGO(m_backGround);
	DeleteGO(m_gameCamera);
	DeleteGO(m_basicEnemy);
	DeleteGO(m_transformEnemy);
	DeleteGO(m_player);
	DeleteGO(m_inGameUI);
}


bool InGame::Start()
{
	InitSky();
	m_backGround = NewGO<FirstPlanet>(0, "BackGround");
	m_player = NewGO<Player>(0, "Player");
	m_basicEnemy = NewGO<BasicEnemy>(0, "BasicEnemy");
	m_transformEnemy = NewGO<TransformEnemy>(0, "TransformEnemy");
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	m_inGameUI = NewGO<InGameUI>(0, "InGameUI");
	return true;
}


void InGame::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_player->SetIsDead(true);
		m_gameCamera->SetPlayerTarget(nullptr);
	}
	// プレイヤーが死亡したらゲームオーバーへ移行。
	if (m_player->GetIsDead()) {
		SceneManager::GetInstance()->ChangeScene(SceneID::GameOver);
	}
}


void InGame::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}