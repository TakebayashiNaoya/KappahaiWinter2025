#include "stdafx.h"
#include "FirstStage.h"
#include "Source/Actor/Planet/FirstPlanet.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Enemy/BasicEnemy/BasicEnemy.h"
#include "Source/Actor/Character/Enemy/DeformEnemy/DeformEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"
#include "Source/Actor/Object/Rocket/Rocket.h"
#include "LoadingScreen.h"


FirstStage::FirstStage()
{

}


FirstStage::~FirstStage()
{
	DeleteGO(m_battleManager);
	DeleteGO(m_skyCube);
	DeleteGO(m_firstStage);
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_inGameUI);
	DeleteGO(m_collisionManager);

	for (auto basicEnemy : m_basicEnemies) {
		if (basicEnemy) {
			DeleteGO(basicEnemy);
		}
	}

	for (auto deformEnemy : m_deformEnemies) {
		if (deformEnemy) {
			DeleteGO(deformEnemy);
		}
	}
}


bool FirstStage::Start()
{
	return true;
}


void FirstStage::Update()
{
	InitObjects();



	//@NOTE: デバッグ用、Bボタンで即死。
	//if (g_pad[0]->IsTrigger(enButtonB)) {
	//	m_titlePlayer->SetIsDead(true);
	//	m_gameCamera->SetPlayerTarget(nullptr);
	//}



	// プレイヤーが死亡したらゲームオーバーへ移行。
	if (m_player) {
		if (m_player->IsDying()) {
			SceneManager::GetInstance()->ChangeScene(SceneID::GameOver);
		}
	}


	if (m_rocket)
	{
		if (m_rocket->IsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Opened) {
			LoadingScreen::StartLoading();
		}
		if (m_rocket->IsGooled() && LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
			SceneManager::GetInstance()->ChangeScene(SceneID::GameClear);
		}
	}
}

void FirstStage::InitObjects()
{
	if (m_loadingState < 7) {
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
			InitLevel();
			m_loadingState = 4;
			return;
		case 4:
			m_inGameUI = NewGO<UIInGame>(0, "UIInGame");
			m_loadingState = 5;
			return;
		case 5:
			m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
			m_loadingState = 6;
			return;
		case 6:
			LoadingScreen::FinishLoading();
			m_loadingState = 7;
			break;
		}
	}
}


void FirstStage::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}


void FirstStage::InitLevel()
{
	m_levelRender.Init("Assets/modelData/stage/FirstPlanet/FirstPlanetLevel.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"FirstPlanet")) {
			m_firstStage = NewGO<FirstPlanet>(0, "FirstPlanet");
			m_firstStage->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"player")) {
			m_player = NewGO<Player>(0, "Player");
			m_player->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"wolf")) {
			BasicEnemy* wolf = NewGO<BasicEnemy>(0, "BasicEnemy");
			wolf->SetTRS(objData.position, objData.rotation, objData.scale);
			m_basicEnemies.push_back(wolf);
			return true;
		}
		if (objData.EqualObjectName(L"spider")) {
			DeformEnemy* spider = NewGO <DeformEnemy>(0, "DeformEnemy");
			spider->SetTRS(objData.position, objData.rotation, objData.scale);
			m_deformEnemies.push_back(spider);
			return true;
		}
		if (objData.EqualObjectName(L"rocket")) {
			m_rocket = NewGO <Rocket>(0, "Rocket");
			m_rocket->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		return false;

		});
}