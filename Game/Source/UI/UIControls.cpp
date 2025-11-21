#include "stdafx.h"
#include "UIControls.h"


namespace
{
	const Vector2 JUMP_IMAGE_SIZE = { 150.0f, 150.0f };
	const Vector2 DASH_IMAGE_SIZE = { 150.0f, 150.0f };
	const Vector2 BUTTON_A_SIZE = { 100.0f, 100.0f };
	const Vector2 BUTTON_B_SIZE = { 100.0f, 100.0f };

	const Vector3 JUMP_IMAGE_POS = { 630.0f, -370.0f, 0.0f };
	const Vector3 DASH_IMAGE_POS = { 800.0f, -300.0f, 0.0f };
	const Vector3 BUTTON_A_POS = { 700.0f, -450.0f, 0.0f };
	const Vector3 BUTTON_B_POS = { 870.0f, -380.0f, 0.0f };
}


UIControls::UIControls()
{
}

UIControls::~UIControls()
{
}

bool UIControls::Start()
{
	m_jumpImage.Init("Assets/sprite/RabbitJump.dds", JUMP_IMAGE_SIZE.x, JUMP_IMAGE_SIZE.y);
	m_jumpImage.SetPosition(JUMP_IMAGE_POS);
	m_jumpImage.Update();
	m_dashImage.Init("Assets/sprite/RabbitDash.dds", DASH_IMAGE_SIZE.x, DASH_IMAGE_SIZE.y);
	m_dashImage.SetPosition(DASH_IMAGE_POS);
	m_dashImage.Update();
	m_buttonA.Init("Assets/sprite/ButtonA.dds", BUTTON_A_SIZE.x, BUTTON_A_SIZE.y);
	m_buttonA.SetPosition(BUTTON_A_POS);
	m_buttonA.Update();
	m_buttonB.Init("Assets/sprite/ButtonB.dds", BUTTON_B_SIZE.x, BUTTON_B_SIZE.y);
	m_buttonB.SetPosition(BUTTON_B_POS);
	m_buttonB.Update();

	return true;
}

void UIControls::Update()
{
}

void UIControls::Render(RenderContext& rc)
{
	m_jumpImage.Draw(rc);
	m_dashImage.Draw(rc);
	m_buttonA.Draw(rc);
	m_buttonB.Draw(rc);
}
