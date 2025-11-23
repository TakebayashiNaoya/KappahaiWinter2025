#include "stdafx.h"
#include "Title.h"
#include "InGame.h"
#include "Source/UI/UITitle.h"
#include "Source/Actor/Character/Player/TitlePlayer.h"
#include "Source/Camera/TitleCamera.h"
#include "Source/Actor/Stage/TitleStage.h"
#include "Source/UI/UITitle.h"
#include "LoadingScreen.h"


Title::Title()
{
	m_titlePlayer = NewGO<TitlePlayer>(0, "TitlePlayer");
	m_titleCamera = NewGO<TitleCamera>(0, "TitleCamera");
	m_titleStage = NewGO<TitleStage>(0, "TitleStage");
	m_uiTitle = NewGO<UITitle>(0, "UITitle");
	InitSky();
}


Title::~Title()
{
	//DeleteGO(m_titleUI);
	DeleteGO(m_titlePlayer);
	DeleteGO(m_titleCamera);
	DeleteGO(m_titleStage);
	DeleteGO(m_uiTitle);
	DeleteGO(m_skyCube);
}


bool Title::Start()
{
	//m_titleUI = NewGO<UITitle>(0, "UITitle");
	LoadingScreen::ChangeState(LoadingScreen::enState_Open);

	return true;
}


void Title::Update()
{
	// Aボタンが押されたらインゲームへ移行。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		LoadingScreen::StartLoading();
	}

	// Loadingになったらシーン切り替えをリクエスト。
	if (LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
		SceneManager::GetInstance()->ChangeScene(SceneID::InGame);
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
