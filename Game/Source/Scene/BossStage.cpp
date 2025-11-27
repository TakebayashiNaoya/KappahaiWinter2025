#include "stdafx.h"
#include "BossStage.h"
#include "Source/Actor/Planet/BossPlanet.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Object/Spawner/Spawner.h"
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/UI/UIBossLife.h"
#include "Source/Battle/BattleManager.h"
#include "Source/Collision/CollisionManager.h"
#include "LoadingScreen.h"

BossStage::BossStage()
{

}

BossStage::~BossStage()
{
	DeleteGO(m_battleManager);
	DeleteGO(m_skyCube);
	DeleteGO(m_bossPlanet);
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_bossEnemy);
	DeleteGO(m_inGameUI);
	DeleteGO(m_collisionManager);

	for (auto spawner : m_spawners) {
		if (spawner) {
			DeleteGO(spawner);
		}
	}
}

bool BossStage::Start()
{
	return true;
}

void BossStage::Update()
{
	InitObjects();

	if (m_bossEnemy->IsDead()) {
		DeleteGO(m_uiBossLife);
	}

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
}

void BossStage::InitObjects()
{
	if (m_loadingState < 8) {
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
			m_uiBossLife = NewGO<UIBossLife>(0, "UIBossLife");
			m_loadingState = 6;
			return;
		case 6:
			m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
			m_loadingState = 7;
			return;
		case 7:
			LoadingScreen::FinishLoading();
			m_loadingState = 8;
			break;
		}
	}
}

void BossStage::InitLevel()
{
	m_levelRender.Init("Assets/modelData/stage/BossPlanet/BossPlanetLevel.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"planet")) {
			m_bossPlanet = NewGO<BossPlanet>(0, "BossPlanet");
			m_bossPlanet->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"player")) {
			m_player = NewGO<Player>(0, "Player");
			m_player->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		if (objData.EqualObjectName(L"spiderSpawner")) {
			Spawner* spider = NewGO <Spawner>(0, "Spawner");
			spider->SetTRS(objData.position, objData.rotation, objData.scale);
			m_spawners.push_back(spider);
			return true;
		}
		if (objData.EqualObjectName(L"bear")) {
			m_bossEnemy = NewGO <BossEnemy>(0, "BossEnemy");
			m_bossEnemy->SetTRS(objData.position, objData.rotation, objData.scale);
			return true;
		}
		return false;

		});
}

void BossStage::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}
