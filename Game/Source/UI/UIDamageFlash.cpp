#include "stdafx.h"
#include "UIDamageFlash.h"
#include "LoadingScreen.h"

namespace
{
	const float SPRITE_SIZE_W = 1920.0f;
	const float SPRITE_SIZE_H = 1080.0f;
}

UIDamageFlash::UIDamageFlash()
{
}

UIDamageFlash::~UIDamageFlash()
{
	// BattleManagerからダメージフラッシュUI登録を解除。
	if (auto BattleMgr = BattleManager::GetInstance()) {
		BattleMgr->UnregisterUIDamageFlash();
	}
}

bool UIDamageFlash::Start()
{
	// BattleManagerにダメージフラッシュUIを登録。
	if (auto BattleMgr = BattleManager::GetInstance()) {
		BattleMgr->RegisterUIDamageFlash(this);
	}

	m_flashSprites[enPlayerCondition_Danger].Init("Assets/sprite/DamageFlash1.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);
	m_flashSprites[enPlayerCondition_Caution].Init("Assets/sprite/DamageFlash2.dds", SPRITE_SIZE_W, SPRITE_SIZE_H);
	return true;
}

void UIDamageFlash::Update()
{
}

void UIDamageFlash::Render(RenderContext& rc)
{
	if (LoadingScreen::GetState() != LoadingScreen::enState_Opened) {
		return;
	}

	if (BattleManager::GetIsBattleFinish()) {
		return;
	}

	// プレイヤーの体力が危険か注意のときだけ描画。
	if (m_playerHp == enPlayerCondition_Danger || m_playerHp == enPlayerCondition_Caution)
	{
		m_flashSprites[m_playerHp].Draw(rc);
	}
}
