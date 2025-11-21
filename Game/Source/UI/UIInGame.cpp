#include "stdafx.h"
#include "UIInGame.h"
#include "UIPlayerLife.h"
#include "UIDamageFlash.h"
#include "UIBossLife.h"
#include "UIControls.h"

namespace
{
	const Vector3 FONT_POSITION = Vector3(700.0f, 500.0f, 0.0f);	//フォントの表示位置。
	const float FONT_SCALE = 1.5f;									//フォントの大きさ。
}

UIInGame::UIInGame()
{
}

UIInGame::~UIInGame()
{
	DeleteGO(m_uiPlayerLife);
	DeleteGO(m_uiDamageFlash);
	DeleteGO(m_uiBossLife);
	DeleteGO(m_uiControls);
}

bool UIInGame::Start()
{
	m_uiPlayerLife = NewGO<UIPlayerLife>(0, "UIPlayerLife");
	m_uiDamageFlash = NewGO<UIDamageFlash>(0, "UIDamageFlash");
	m_uiBossLife = NewGO<UIBossLife>(0, "UIBossLife");
	m_uiControls = NewGO<UIControls>(0, "UIControls");


	//m_lifeFR.SetPosition(FONT_POSITION);	//配置位置
	//m_lifeFR.SetScale(FONT_SCALE);			//大きさ
	//m_lifeFR.SetColor(g_vec4Yellow);		//色
	return true;
}

void UIInGame::Update()
{
	////ライフの数を表示する。
	//wchar_t starText[256];
	//swprintf_s(starText, 256, L"LIFE:%d", m_lifeCounts);
	//m_lifeFR.SetText(starText);

}

void UIInGame::Render(RenderContext& rc)
{
	//m_lifeFR.Draw(rc);
}
