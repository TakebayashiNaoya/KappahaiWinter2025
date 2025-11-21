#include "stdafx.h"
#include "UITitle.h"

UITitle::UITitle()
{
	m_backGroundImage.Init("Assets/sprite/Title.dds", 1920.0f, 1080.0f);
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
}

void UITitle::Render(RenderContext& rc)
{
	m_backGroundImage.Draw(rc);
}
