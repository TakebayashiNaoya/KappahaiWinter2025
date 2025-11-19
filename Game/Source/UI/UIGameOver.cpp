#include "stdafx.h"
#include "UIGameOver.h"

UIGameOver::UIGameOver()
{
	m_gameOverImage.Init("Assets/sprite/GameOver.dds", 1920.0f, 1080.0f);
}

UIGameOver::~UIGameOver()
{
}

bool UIGameOver::Start()
{
	return true;
}

void UIGameOver::Update()
{
}

void UIGameOver::Render(RenderContext& rc)
{
	//if (m_isDrawStop) {
	//	return;
	//}
	m_gameOverImage.Draw(rc);
}
