#include "stdafx.h"
#include "UIInGame.h"
#include "UIPlayerLife.h"
#include "UIDamageFlash.h"
#include "UIBossLife.h"
#include "UIControls.h"


UIInGame::UIInGame()
{
}

UIInGame::~UIInGame()
{
	DeleteGO(m_uiPlayerLife);
	DeleteGO(m_uiDamageFlash);
	DeleteGO(m_uiControls);
}

bool UIInGame::Start()
{
	m_uiPlayerLife = NewGO<UIPlayerLife>(0, "UIPlayerLife");
	m_uiDamageFlash = NewGO<UIDamageFlash>(0, "UIDamageFlash");
	m_uiControls = NewGO<UIControls>(0, "UIControls");

	return true;
}

void UIInGame::Update()
{
}

void UIInGame::Render(RenderContext& rc)
{
}
