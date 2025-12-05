#include "stdafx.h"
#include "UIGear.h"
#include "TextTemplate.h"
#include "LoadingScreen.h"


namespace
{
	ResultInfo GOT_GEAR_COUNT =
	{
		std::string(""),
		Vector3(-770.0f, 490.0f, 0.0f),
		float(1.8f),
		Vector4(g_vec4White)
	};
}


UIGear::UIGear()
{
}


UIGear::~UIGear()
{
}


bool UIGear::Start()
{
	m_gearImage.Init("Assets/sprite/Gear.dds", 150.0f, 150.0f);
	m_gearImage.SetPosition({ -850.0f, 450.0f, 0.0f });
	m_gearImage.Update();
	return true;
}


void UIGear::Update()
{
	SetTextOption(&m_gotGearCountFR, GOT_GEAR_COUNT, L"%d/%d", m_gotGearCount, m_maxGearCount);
}


void UIGear::Render(RenderContext& rc)
{
	if (LoadingScreen::GetState() != LoadingScreen::enState_Opened) {
		return;
	}

	if (BattleManager::GetIsBattleFinish()) {
		return;
	}

	m_gearImage.Draw(rc);
	m_gotGearCountFR.Draw(rc);
}
