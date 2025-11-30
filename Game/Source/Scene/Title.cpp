#include "stdafx.h"
#include "Title.h"
#include "Source/UI/UITitle.h"
#include "Source/Actor/Character/Player/TitlePlayer.h"
#include "Source/Camera/TitleCamera.h"
#include "Source/Actor/Planet/TitlePlanet.h"
#include "LoadingScreen.h"


Title::Title()
{
	m_titlePlayer = NewGO<TitlePlayer>(0, "TitlePlayer");
	m_titleCamera = NewGO<TitleCamera>(0, "TitleCamera");
	m_titleStage = NewGO<TitlePlanet>(0, "TitlePlanet");
	m_uiTitle = NewGO<UITitle>(0, "UITitle");
}


Title::~Title()
{
	SoundManager::StopBGM(enSoundList_TitleBGM, 1.0f);
	DeleteGO(m_titlePlayer);
	DeleteGO(m_titleCamera);
	DeleteGO(m_titleStage);
	DeleteGO(m_uiTitle);
	DeleteGO(m_skyCube);
}


bool Title::Start()
{
	InitSky();
	m_waitTimer = 0.0f;
	m_isLoadingFinished = false;

	return true;
}


void Title::Update()
{
	// ロード画面を綺麗に見せるため、少し待機。
	if (m_isLoadingFinished == false) {
		m_waitTimer += g_gameTime->GetFrameDeltaTime();

		if (m_waitTimer > 0.1f) {
			LoadingScreen::FinishLoading();
			m_isLoadingFinished = true;
			SoundManager::Play(enSoundList_TitleBGM, true);
			SoundManager::SetVolume(enSoundList_FirstStageBGM, 0.0f);
		}
		return;
	}



	// Aボタンが押されたらインゲームへ移行。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		SoundManager::Play(enSoundList_PushSE);
		// ロードオープン中にボタンを押したら、ロード完了へ。
		if (LoadingScreen::GetState() == LoadingScreen::enState_Open) {
			LoadingScreen::ChangeState(LoadingScreen::enState_Opened);
		}
		// ロード完了時にボタンを押したら、ロード開始へ。
		else if (LoadingScreen::GetState() == LoadingScreen::enState_Opened) {
			LoadingScreen::StartLoading();
		}
	}

	// Loadingになったらシーン切り替えをリクエスト。
	if (LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
		SceneManager::GetInstance()->ChangeScene(SceneID::FirstStage);
	}
}

void Title::InitSky()
{
	// 現在の空を破棄。
	DeleteGO(m_skyCube);

	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)m_skyCubeType);

	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.1f);
}
