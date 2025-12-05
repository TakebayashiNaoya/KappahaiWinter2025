#include "stdafx.h"
#include "BattleStageBase.h"
#include "Source/Actor/Character/Player/Player.h"   
#include "Source/Actor/Character/Enemy/BossEnemy/BossEnemy.h"
#include "Source/UI/UIGameOver.h"
#include "Source/UI/UIGameClear.h"
#include "Source/Camera/GameCamera.h"
#include "Source/UI/UIInGame.h"
#include "Source/Collision/CollisionManager.h"
#include "LoadingScreen.h"
#include "nature/SkyCube.h"

BattleStageBase::BattleStageBase()
{
	BattleManager::SetIsBattleFinish(false);
	BattleManager::SetIsStopCollisionManager(false);
	RegisterLoadingTasks();
}

BattleStageBase::~BattleStageBase()
{
	BattleManager::GetInstance()->DestroyAllEnemies();

	DeleteGO(m_skyCube);

	DeleteGO(m_inGameUI);

	DeleteGO(m_gameCamera);

	BattleManager::GetInstance()->Unregister(m_player);
	DeleteGO(m_player);
}

bool BattleStageBase::Start()
{
	return true;
}

void BattleStageBase::Update()
{
	InitObjects();

	OnUpdate();

	switch (m_battlePhase)
	{
		// 1.プレイヤーかボスが死亡したら、勝敗のステートを設定する。
	case enBattlePhase_Battle:
		if (m_player && m_player->IsDying()) {
			m_result = enResult_PlayerLose;
			m_battlePhase = enBattlePhase_BattleFinish;
		}
		else if (m_bossEnemy && m_bossEnemy->IsDying()) {
			m_result = enResult_PlayerWin;
			m_battlePhase = enBattlePhase_BattleFinish;
		}
		break;


		// 2.相打ちにならないようにコリジョンマネージャーを破棄する。
	case enBattlePhase_BattleFinish:
		BattleManager::SetIsStopCollisionManager(true);
		m_battlePhase = enBattlePhase_WaitFinishAnimation;
		break;


		// 3.プレイヤーの死亡アニメーションが終わったらゲームオーバーへ、
		//   ボスの死亡アニメーションが終わったらゲームクリアへ。
	case enBattlePhase_WaitFinishAnimation:
		if (m_result == enResult_PlayerLose) {
			if (m_player && m_player->GetIsDead()) {
				BattleManager::SetIsBattleFinish(true);
				m_battlePhase = enBattlePhase_GameOver;
			}
		}
		else if (m_result == enResult_PlayerWin) {
			if (m_bossEnemy && m_bossEnemy->GetIsDead()) {
				BattleManager::SetIsBattleFinish(true);
				m_battlePhase = enBattlePhase_GameClear;
			}
		}
		break;


		// 4.ゲームオーバー、またはゲームクリアUIを表示する。
	case enBattlePhase_GameOver:
		if (m_uiResult == nullptr) {
			m_uiResult = NewGO<UIGameOver>(0, "UIGameOver");
		}
		m_battlePhase = enBattlePhase_WaitEnd;
		break;
	case enBattlePhase_GameClear:
		if (m_uiResult == nullptr) {
			m_uiResult = NewGO<UIGameClear>(0, "UIGameClear");
		}
		m_battlePhase = enBattlePhase_WaitEnd;
		break;


		// 5.ゲームオーバー、またはゲームクリアUIの終了を待ってタイトルへ戻る。
	case enBattlePhase_WaitEnd:
		if (m_uiResult->GetIsEnd()) {
			// ロード画面へ移行。
			if (LoadingScreen::GetState() != LoadingScreen::enState_Loading) {
				LoadingScreen::ChangeState(LoadingScreen::enState_Loading);
			}

			if (LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
				// 黒画像が残ってしまっているので破棄する。
				if (m_uiResult) {
					DeleteGO(m_uiResult);
					m_uiResult = nullptr;
				}
				m_battlePhase = enBattlePhase_End;
			}
		}
		break;

	case enBattlePhase_End:
		SceneManager::GetInstance()->ChangeScene(SceneID::Title);
		break;
	}
}


void BattleStageBase::InitObjects()
{
	if (m_currentTaskIndex < m_loadingTasks.size()) {
		m_loadingTasks[m_currentTaskIndex]();
		m_currentTaskIndex++;
	}
}

void BattleStageBase::RegisterLoadingTasks()
{
	//m_loadingTasks.push_back([this]() {
	//	m_battleManager = NewGO<BattleManagerObject>(0, "BattleManagerObject");
	//	});

	//m_loadingTasks.push_back([this]() {
	//	m_collisionManager = NewGO<CollisionManagerObject>(0, "CollisionManagerObject");
	//	});

	m_loadingTasks.push_back([this]() {
		InitSky();
		});

	m_loadingTasks.push_back([this]() {
		InitLevel();
		});

	m_loadingTasks.push_back([this]() {
		m_inGameUI = NewGO<UIInGame>(0, "UIInGame");
		});

	m_loadingTasks.push_back([this]() {
		m_gameCamera = NewGO<GameCamera>(0, "GameCamera");
		});

	m_loadingTasks.push_back([this]() {
		LoadingScreen::FinishLoading();
		});
}


void BattleStageBase::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}
