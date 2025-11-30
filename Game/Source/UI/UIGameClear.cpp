#include "stdafx.h"
#include "UIGameClear.h"
#include <cmath> 
#include "LoadingScreen.h"

namespace
{
	const Vector3 START_POS = Vector3(0.0f, -400.0f, 0.0f);	// スタート地点
	const Vector3 TARGET_POS = Vector3(0.0f, 300.0f, 0.0f);	// 着地地点

	// ジャンプの高さ設定（スタート地点またはゴール地点の高い方から、さらにこれだけ高く飛ぶ）
	constexpr float JUMP_HEIGHT_OFFSET = 200.0f;

	// 物理設定（小さく速く跳ねる設定）
	constexpr float GRAVITY = 8000.0f;
	constexpr float BOUNCE_COEFFICIENT = 0.7f;
	constexpr float STOP_VELOCITY_THRESHOLD = 50.0f;

	// 時間設定
	constexpr float WAIT_TIME = 1.0f;
	constexpr float SHRINK_TIME = 0.5f;

	// 回転設定 
	// 収縮中に何回転させるか。360度(2PI) * 回転数 / 時間
	// 時計回りなのでマイナスにします。
	// 例: 0.5秒で2回転させる場合 -> 720度 / 0.5秒 = 1440度/秒
	constexpr float SHRINK_ROTATE_SPEED = 1440.0f;
	constexpr float PAI = 3.14159265f;
}

UIGameClear::UIGameClear()
{
	m_gameClearImage.Init("Assets/sprite/GameClear.dds", 1000.0f, 300.0f);
	m_alpha = 0.0f;
	m_gameClearImage.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
}

UIGameClear::~UIGameClear()
{
}

bool UIGameClear::Start()
{
	// 1. 初期座標セット
	m_position = START_POS;
	m_scale = Vector3::One;
	m_alpha = 0.0f;

	// 回転初期化 (NEW)
	m_rotation = Quaternion::Identity;
	m_angle = 0.0f;

	// 2. 物理計算（初速度の決定）

	// 最高到達点（Y座標）を計算
	// スタートとゴール、高い方を選んで、そこからさらにOFFSET分高くする
	float startY = START_POS.y;
	float targetY = TARGET_POS.y;
	float peakY = (startY > targetY ? startY : targetY) + JUMP_HEIGHT_OFFSET;

	// A. 上昇にかかる時間 (t_up) と初速 (vy) を計算
	// v = sqrt(2 * g * h)
	float heightUp = peakY - startY;
	float velocityY0 = sqrtf(2.0f * GRAVITY * heightUp);
	float timeUp = velocityY0 / GRAVITY;

	// B. 下降にかかる時間 (t_down) を計算
	// h = 1/2 * g * t^2  =>  t = sqrt(2h / g)
	float heightDown = peakY - targetY;
	float timeDown = sqrtf(2.0f * heightDown / GRAVITY);

	// C. 合計滞空時間
	float totalTime = timeUp + timeDown;

	// D. 横方向の速度 (vx, vz) を計算
	// 距離 / 時間 = 速度
	Vector3 distance = TARGET_POS - START_POS;
	m_velocity.x = distance.x / totalTime;
	m_velocity.z = distance.z / totalTime; // 奥行きがある場合用
	m_velocity.y = velocityY0;             // 縦方向の初速

	// ステート開始
	m_state = enState_JumpArc;

	m_gameClearImage.SetRotation(m_rotation); // (NEW)
	m_gameClearImage.SetPosition(m_position);
	m_gameClearImage.Update();

	SoundManager::Play(enSoundList_GameClear);

	return true;
}

void UIGameClear::Update()
{
	float dt = g_gameTime->GetFrameDeltaTime();

	switch (m_state)
	{
		// ロゴが上昇して、着地するまでの放物線ジャンプ。
	case enState_JumpArc:
	{
		// 1. 重力加算 (Y軸のみ)
		m_velocity.y -= GRAVITY * dt;

		// 2. 移動 (X, Y, Z 全て動く)
		m_position += m_velocity * dt;

		// 3. フェードイン演出 (上昇中のみ濃くする)
		if (m_velocity.y > 0.0f) {
			// 現在のY速度 / 初速 の割合などで計算、またはシンプルに上昇中は濃くしていく
			// ここではシンプルに「不透明度 += 時間」で濃くします
			m_alpha += dt * 3.0f; // 0.3秒くらいで全表示
			if (m_alpha > 1.0f) m_alpha = 1.0f;
		}

		// 4. 着地判定
		// 「ゴール地点の高さより低く」かつ「落下中(速度がマイナス)」の場合
		if (m_position.y <= TARGET_POS.y && m_velocity.y < 0.0f)
		{
			// 強制的にゴール座標へ補正
			m_position = TARGET_POS;

			// バウンドステートへ移行
			// ※横移動(X, Z)はここでストップさせる
			m_velocity.x = 0.0f;
			m_velocity.z = 0.0f;

			// Y速度を反転・減衰
			m_velocity.y *= -BOUNCE_COEFFICIENT;

			m_state = enState_Bouncing;
		}
		break;
	}

	// その場バウンド。
	case enState_Bouncing:
	{
		m_velocity.y -= GRAVITY * dt;
		m_position.y += m_velocity.y * dt;

		// 接地判定
		if (m_position.y <= TARGET_POS.y) {
			m_position.y = TARGET_POS.y;
			m_velocity.y *= -BOUNCE_COEFFICIENT;

			// ほぼ止まったら終了
			if (abs(m_velocity.y) < STOP_VELOCITY_THRESHOLD) {
				m_velocity.y = 0.0f;
				m_state = enState_Wait;
				m_timer = 0.0f;
			}
		}
		break;
	}

	case enState_Wait:
	{
		m_timer += dt;
		if (m_timer >= WAIT_TIME) {
			m_timer = 0.0f;
			LoadingScreen::StartLoading();
			m_state = enState_Shrink;
		}
		break;
	}

	// 回転させながら縮める。
	case enState_Shrink:
	{
		float shrinkAmount = dt / SHRINK_TIME;
		m_scale.x -= shrinkAmount;
		m_scale.y -= shrinkAmount;
		m_scale.z -= shrinkAmount;

		// 回転処理。
		float rotateRadSpeed = SHRINK_ROTATE_SPEED * (PAI / 180.0f);
		m_angle -= rotateRadSpeed * dt;

		// 角度からクォータニオンを作成 (Z軸回転)。
		float halfAngle = m_angle * 0.5f;
		float qz = sinf(halfAngle);
		float qw = cosf(halfAngle);
		m_rotation = Quaternion(0.0f, 0.0f, qz, qw);

		// 完全に縮んだら終了。
		if (m_scale.x <= 0.0f) {
			m_scale = Vector3::Zero;
			m_state = enState_LoadingWait;
		}
		break;
	}

	case enState_LoadingWait:
	{
		if (LoadingScreen::GetState() == LoadingScreen::enState_Loading) {
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

	m_gameClearImage.SetPosition(m_position);
	m_gameClearImage.SetScale(m_scale);
	m_gameClearImage.SetRotation(m_rotation);
	m_gameClearImage.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
	m_gameClearImage.Update();
}

void UIGameClear::Render(RenderContext& rc)
{
	m_gameClearImage.Draw(rc);
}