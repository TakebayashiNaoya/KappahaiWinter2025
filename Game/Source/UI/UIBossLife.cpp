#include "stdafx.h"
#include "UIBossLife.h"

namespace
{
	const Vector2 SCALE_NAME_TEXT = { 180.0f, 50.0f };
	const Vector2 SCALE_HP_BAR_BACK = { 1071.0f, 107.0f };
	const Vector2 SCALE_HP_BAR_FRONT = { 937.0f, 37.0f };

	const Vector3 POS_NAME_TEXT = { 0.0f, 480.0f, 0.0f };
	const Vector3 POS_HP_BAR_BACK = { 0.0f, 400.0f, 0.0f };
	const Vector3 POS_HP_BAR_FRONT = { -420.0f, 400.0f, 0.0f };

	const Vector2 PIVOT_LEFT_CENTER = { 0.0f, 0.5f };

	const Vector4 COLOR_GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
	const Vector4 COLOR_YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	const Vector4 COLOR_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
}

UIBossLife::UIBossLife()
{
}

UIBossLife::~UIBossLife()
{
}

bool UIBossLife::Start()
{
	// ボス名テキストの初期化。
	m_bossNameText.Init("Assets/sprite/Boss.dds", SCALE_NAME_TEXT.x, SCALE_NAME_TEXT.y);
	m_bossNameText.SetPosition(POS_NAME_TEXT);
	m_bossNameText.Update();

	// ボスHPバー背景の初期化。
	m_bossHpBarBack.Init("Assets/sprite/BossHpBarBack.dds", SCALE_HP_BAR_BACK.x, SCALE_HP_BAR_BACK.y);
	m_bossHpBarBack.SetPosition(POS_HP_BAR_BACK);
	m_bossHpBarBack.Update();

	// ボスHPバー前景の初期化。	
	// 体力の現象に応じて縮めるため、ピボットを左中央に設定。
	m_bossHpBarFront.SetPivot(PIVOT_LEFT_CENTER);
	m_bossHpBarFront.Init("Assets/sprite/BossHpBarFront.dds", SCALE_HP_BAR_FRONT.x, SCALE_HP_BAR_FRONT.y);
	m_bossHpBarFront.SetPosition(POS_HP_BAR_FRONT);

	// 初期状態は緑色に設定。
	m_bossHpBarFront.SetMulColor(COLOR_GREEN);
	m_bossHpBarFront.Update();



	return true;
}

void UIBossLife::Update()
{
	// HPバーのスケールを更新。
	if (m_maxLife > 0)
	{
		// SetScaleは0.0ｆ~1.0fで指定する。
		float hpRatio = static_cast<float>(m_currentLife) / static_cast<float>(m_maxLife);
		Vector3 scale = m_bossHpBarFront.GetScale();
		scale.x = hpRatio;
		m_bossHpBarFront.SetScale(scale);
		// HPバーの色を更新。
		if (hpRatio > 0.5f)
		{
			// 緑色。
			m_bossHpBarFront.SetMulColor(COLOR_GREEN);
		}
		else if (hpRatio > 0.2f)
		{
			// 黄色。
			m_bossHpBarFront.SetMulColor(COLOR_YELLOW);
		}
		else
		{
			// 赤色。
			m_bossHpBarFront.SetMulColor(COLOR_RED);
		}
		m_bossHpBarFront.Update();
	}
}

void UIBossLife::Render(RenderContext& rc)
{
	m_bossNameText.Draw(rc);
	m_bossHpBarBack.Draw(rc);
	m_bossHpBarFront.Draw(rc);
}
