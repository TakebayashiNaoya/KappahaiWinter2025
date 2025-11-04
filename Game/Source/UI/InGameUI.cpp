#include "stdafx.h"
#include "InGameUI.h"

namespace
{
	const Vector3 FONT_POSITION = Vector3(700.0f, 500.0f, 0.0f);	//フォントの表示位置。
	const float FONT_SCALE = 1.5f;									//フォントの大きさ。
}

InGameUI::InGameUI()
{
}

InGameUI::~InGameUI()
{
}

bool InGameUI::Start()
{
	m_lifeFR.SetPosition(FONT_POSITION);	//配置位置
	m_lifeFR.SetScale(FONT_SCALE);			//大きさ
	m_lifeFR.SetColor(g_vec4Yellow);		//色
	return true;
}

void InGameUI::Update()
{
	//ライフの数を表示する。
	wchar_t starText[256];
	swprintf_s(starText, 256, L"LIFE:%d", m_lifeCounts);
	m_lifeFR.SetText(starText);

}

void InGameUI::Render(RenderContext& rc)
{
	m_lifeFR.Draw(rc);
}
