#include "stdafx.h"
#include "TitleUI.h"

TitleUI::TitleUI()
{
	m_backGroundImage.Init("Assets/sprite/Title.dds", 1920.0f, 1080.0f);
}

TitleUI::~TitleUI()
{
}

bool TitleUI::Start()
{
	return true;
}

void TitleUI::Update()
{
}

void TitleUI::Render(RenderContext& rc)
{
	//if (m_isDrawStop) {
	//	return;
	//}
	m_backGroundImage.Draw(rc);
}
