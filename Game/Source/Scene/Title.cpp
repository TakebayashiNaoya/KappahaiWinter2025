#include "stdafx.h"
#include "Title.h"
#include "InGame.h"
#include "Source/UI/TitleUI.h"


Title::Title()
{

}


Title::~Title()
{
	DeleteGO(m_titleUI);
}


bool Title::Start()
{
	m_titleUI = NewGO<TitleUI>(0, "TitleUI");
	return true;
}


void Title::Update()
{
	// Aボタンが押されたらインゲームへ移行。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		//m_titleUI->SetIsDraw(true);
		SceneManager::GetInstance()->ChangeScene(SceneID::InGame);
	}
}