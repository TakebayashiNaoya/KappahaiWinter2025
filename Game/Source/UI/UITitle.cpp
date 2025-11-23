#include "stdafx.h"
#include "UITitle.h"
#include "LoadingScreen.h"

UITitle::UITitle()
{
	m_backGroundImage.Init("Assets/sprite/TitleLogo.dds", 1000.0f, 600.0f);
	m_backGroundImage.SetPosition(Vector3(-400.0f, 100.0f, 0.0f));
	m_backGroundImage.Update();
}

UITitle::~UITitle()
{
}

bool UITitle::Start()
{
	return true;
}

void UITitle::Update()
{
	if (LoadingScreen::GetState() == LoadingScreen::enState_Close)
	{
		DeleteGO(this);
	}
}

void UITitle::Render(RenderContext& rc)
{
	if (LoadingScreen::GetState() == LoadingScreen::enState_Opened) {
		m_backGroundImage.Draw(rc);
	}
}
