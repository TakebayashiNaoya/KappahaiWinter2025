#include "stdafx.h"
#include "InGame.h"
#include "Source/Actor/Stage/FirstStage.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"
#include "LoadingScreen.h"


InGame::InGame()
{

}


InGame::~InGame()
{
	DeleteGO(m_battleManager);
	DeleteGO(m_skyCube);
	DeleteGO(m_firstStage);
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
	return true;
}


void InGame::Update()
{
	InitObjects();

	//@NOTE: デバッグ用、Bボタンで即死。
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	m_titlePlayer->SetIsDead(true);
	//	m_gameCamera->SetPlayerTarget(nullptr);
	//}
	// プレイヤーが死亡したらゲームオーバーへ移行。
	if (m_player == nullptr) {
		return;
	}
	if (m_player->IsDying()) {
		SceneManager::GetInstance()->ChangeScene(SceneID::GameOver);
	}
}

void InGame::InitObjects()
{
	if (m_loadingState < 11) {
		switch (m_loadingState) {
		case 0:
			m_battleManager = NewGO<BattleManagerObject>(0, "BattleManagerObject");
			m_loadingState = 1;
			return;
		case 1:
			m_collisionManager = NewGO<CollisionManagerObject>(0, "CollisionManagerObject");
			m_loadingState = 2;
			return;
		case 2:
			InitSky();
			m_loadingState = 3;
			return;
		case 3:
			m_firstStage = NewGO<FirstStage>(0, "FirstStage");
			m_loadingState = 4;
			return;
		case 4:
			m_player = NewGO<Player>(0, "Player");
			m_loadingState = 5;
			return;
		case 5:
			m_basicEnemy = NewGO<BasicEnemy>(0, "BasicEnemy");
			m_loadingState = 6;
			return;
		case 6:
			m_deformEnemy = NewGO<DeformEnemy>(0, "DeformEnemy");
			m_loadingState = 7;
			return;
		case 7:
			m_bossEnemy = NewGO<BossEnemy>(0, "BossEnemy");
			m_loadingState = 8;
			return;
		case 8:
			m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
			m_loadingState = 9;
			return;
		case 9:
			m_inGameUI = NewGO<UIInGame>(0, "UIInGame");
			m_loadingState = 10;
			return;
		case 10:
			LoadingScreen::FinishLoading();
			m_loadingState = 11;
			break;
		}
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