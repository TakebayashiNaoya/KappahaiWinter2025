#include "stdafx.h"
#include "UIGameOver.h"

namespace
{
	constexpr float FADE_IN_TIME = 1.0f;			// フェードインにかかる時間。
	constexpr float IMAGE_CUTIN_TIME = 1.5f;		// ゲームオーバー画像を動かす時間。
	constexpr float IMAGE_FIRST_HEIGHT = 1500.0f;	// ゲームオーバー画像の初期位置。	
	constexpr float ROTATION_COUNT = 4.0f;			// 回転する回数。
	constexpr float PAI = 3.14159265f;				// 円周率。

	constexpr float BEFORE_SLIP_WAIT_TIME = 0.5f;	// スリップ前の待機時間。
	constexpr float AFTER_SLIP_WAIT_TIME = 1.0f;	// スリップ後の待機時間。

	constexpr float SLIP_ROTATION_TIME = 0.1f;	// 何秒かけて傾けるか
	constexpr float SLIP_TARGET_ANGLE = 20.0f;	// 最終的に何度まで傾けるか（度数法）
	constexpr float SLIP_DROP_DISTANCE = 150.0f; // 傾くときに下にずれる距離
}

UIGameOver::UIGameOver()
{
	m_hideImage.Init("Assets/sprite/LoadingImage_Around.dds", 1920.0f, 1080.0f);
	m_hideImage.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	m_gameOverImage.Init("Assets/sprite/GameOver.dds", 700.0f, 200.0f);
	m_gameOverImageAlpha = 1.0f;
}

UIGameOver::~UIGameOver()
{
}

bool UIGameOver::Start()
{
	m_position.y = IMAGE_FIRST_HEIGHT;
	m_gameOverImage.SetPosition(m_position);
	m_gameOverImage.Update();
	return true;
}

void UIGameOver::Update()
{
	switch (m_state)
	{
	case enState_HideImageFadeIn:
	{
		// フェードイン処理。
		m_hideImageAlpha += g_gameTime->GetFrameDeltaTime() / FADE_IN_TIME;
		if (m_hideImageAlpha >= 1.0f) {
			m_hideImageAlpha = 1.0f;
			m_state = enState_EntryImage;
		}
		m_hideImage.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, m_hideImageAlpha));
		m_hideImage.Update();
		break;
	}

	case enState_EntryImage:
	{
		// ゲームオーバー画像のカットイン処理。
		m_entryTimer += g_gameTime->GetFrameDeltaTime() / IMAGE_CUTIN_TIME;
		float entryRate = 1.0f - m_entryTimer;

		m_position.y = IMAGE_FIRST_HEIGHT * entryRate;

		float angle = entryRate * PAI * 2.0f * ROTATION_COUNT;

		float halfAngle = angle * 0.5f;
		float qz = sinf(halfAngle);
		float qw = cosf(halfAngle);
		m_rotation = Quaternion(0.0f, 0.0f, qz, qw);

		if (m_entryTimer >= 1.0f) {
			m_entryTimer = 1.0f;
			entryRate = 0.0f;
			m_rotation = Quaternion::Identity;
			m_position = Vector3::Zero;
			m_state = enState_WaitBeforeSlip;
		}
		break;
	}

	case enState_WaitBeforeSlip:
	{
		// スリップ前の待機処理。
		m_waitTimer += g_gameTime->GetFrameDeltaTime();
		if (m_waitTimer >= BEFORE_SLIP_WAIT_TIME) {
			m_waitTimer = 0.0f;
			m_state = enState_SlipImage;
		}
		break;
	}

	case enState_SlipImage:
	{
		// 1. タイマー加算
		m_slipTimer += g_gameTime->GetFrameDeltaTime();

		// 2. 進捗率（0.0 ～ 1.0）を計算
		float rotateRate = m_slipTimer / SLIP_ROTATION_TIME;
		if (rotateRate > 1.0f) {
			rotateRate = 1.0f;
		}

		// 3. 回転計算
		// 進捗率に応じて角度をつける
		float currentDeg = SLIP_TARGET_ANGLE * rotateRate * -1.0f;
		float angleRad = currentDeg * (PAI / 180.0f); // ラジアン変換

		float halfAngle = angleRad * 0.5f;
		float qz = sinf(halfAngle);
		float qw = cosf(halfAngle);
		m_rotation = Quaternion(0.0f, 0.0f, qz, qw);


		// 4. 移動計算（ここを追加）
		// 「0」から「設定した距離(マイナス方向)」へ、回転と同じペースで移動
		m_position.y = -SLIP_DROP_DISTANCE * rotateRate;


		// 5. 終了判定（rotateRateが1になったら）
		if (rotateRate >= 1.0f) {
			m_position.y = -SLIP_DROP_DISTANCE;
			m_state = enState_WaitAfterSlip;
		}
		break;
	}

	case enState_WaitAfterSlip:
	{
		// スリップ後の待機処理。
		m_waitTimer += g_gameTime->GetFrameDeltaTime();
		if (m_waitTimer >= AFTER_SLIP_WAIT_TIME) {
			m_waitTimer = 0.0f;
			m_state = enState_GameOverImageFadeOut;
		}
		break;
	}

	case enState_GameOverImageFadeOut:
	{
		// ゲームオーバー画像のフェードアウト処理。
		m_gameOverImageAlpha -= g_gameTime->GetFrameDeltaTime() / FADE_IN_TIME;
		m_gameOverImage.SetMulColor(Vector4(m_gameOverImageAlpha, m_gameOverImageAlpha, m_gameOverImageAlpha, m_gameOverImageAlpha));
		if (m_gameOverImageAlpha <= 0.0f) {
			m_gameOverImageAlpha = 0.0f;
			m_state = enState_End;
		}
		break;
	}

	case enState_End:
	{
		m_isEnd = true;
		break;
	}
	}
	m_gameOverImage.SetRotation(m_rotation);
	m_gameOverImage.SetPosition(m_position);
	m_gameOverImage.Update();

}
void UIGameOver::Render(RenderContext& rc)
{
	m_hideImage.Draw(rc);
	m_gameOverImage.Draw(rc);
}
