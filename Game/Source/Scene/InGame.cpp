#include "stdafx.h"
#include "InGame.h"
#include "Source/Actor/Stage/BackGround.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"


InGame::InGame()
{

}


InGame::~InGame()
{
	DeleteGO(m_battleManager);
	DeleteGO(m_skyCube);
	DeleteGO(m_backGround);
	DeleteGO(m_gameCamera);
	DeleteGO(m_basicEnemy);
	DeleteGO(m_deformEnemy);
	DeleteGO(m_bossEnemy);
	DeleteGO(m_player);
	DeleteGO(m_inGameUI);
	DeleteGO(m_collisionManager);
}


bool InGame::Start()
{
	m_battleManager = NewGO<BattleManagerObject>(0, "BattleManagerObject");
	m_collisionManager = NewGO<CollisionManagerObject>(0, "CollisionManagerObject");
	InitSky();
	m_backGround = NewGO<FirstPlanet>(0, "BackGround");
	m_player = NewGO<Player>(0, "Player");
	m_basicEnemy = NewGO<BasicEnemy>(0, "BasicEnemy");
	m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
	m_bossEnemy = NewGO<BossEnemy>(0, "BossEnemy");
	m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
	m_inGameUI = NewGO<UIInGame>(0, "UIInGame");
	return true;
}


void InGame::Update()
{
	//@NOTE: デバッグ用、Bボタンで即死。
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	m_player->SetIsDead(true);
	//	m_gameCamera->SetPlayerTarget(nullptr);
	//}
	// プレイヤーが死亡したらゲームオーバーへ移行。
	if (m_player->IsDying()) {
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