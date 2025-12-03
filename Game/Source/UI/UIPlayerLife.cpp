#include "stdafx.h"
#include "UIPlayerLife.h"
#include "LoadingScreen.h"

namespace
{
	const Vector3 DISPLAY_POSITION = Vector3(800.0f, 400.0f, 0.0f);
	const float SPRITE_SIZE_W = 300.0f;
	const float SPRITE_SIZE_H = 300.0f;
}

UIPlayerLife::UIPlayerLife()
{
}

UIPlayerLife::~UIPlayerLife()
{
	if (auto bm = BattleManager::GetInstance()) {
		bm->UnregisterUIPlayerLife();
	}
}

bool UIPlayerLife::Start()
{
	// バトルマネージャーに自分を登録。
	if (auto bm = BattleManager::GetInstance()) {
		bm->RegisterUIPlayerLife(this);
	}

	m_hpSprites[enPlayerCondition_Dead].Init("Assets/sprite/HP0.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);
	m_hpSprites[enPlayerCondition_Danger].Init("Assets/sprite/HP1.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);
	m_hpSprites[enPlayerCondition_Caution].Init("Assets/sprite/HP2.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);
	m_hpSprites[enPlayerCondition_Fine].Init("Assets/sprite/HP3.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);

	for (auto& sprite : m_hpSprites) {
		sprite.SetPosition(DISPLAY_POSITION);
		sprite.Update();
	}

	m_displayHpSprite = &m_hpSprites[enPlayerCondition_Fine];

	return true;
}

void UIPlayerLife::Update()
{
}

void UIPlayerLife::Render(RenderContext& rc)
{
	if (LoadingScreen::GetState() != LoadingScreen::enState_Opened) {
		return;
	}

	if (BattleManager::GetIsBattleFinish()) {
		return;
	}

	m_displayHpSprite = &m_hpSprites[m_playerHp];
	m_displayHpSprite->Draw(rc);
}
