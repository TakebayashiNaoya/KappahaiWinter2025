#include "stdafx.h"
#include "TitlePlayer.h"
#include "Source/Scene/SceneManager.h"
#include "Source/Camera/TitleCamera.h"

const Character::AnimationOption TitlePlayer::TITLE_PLAYER_ANIMATION_OPTIONS[] = {
   {"Player/idle",	true},
   {"Player/walk",	true},
   {"Player/run",	true},
   {"Player/down",	true},
   {"Player/dead",	false},
};

namespace
{
	const std::string MODEL_PATH = "Player/rabbit";
	constexpr float MODEL_SCALE = 200.0f;

	const Vector3 SPAWN_POSITION = Vector3(0.0f, 1000.0f, 0.0f);

	//const Vector3 MOVE_DIRECTION = Vector3(-0.5f, -0.5f, 0.0f);
	//constexpr float WALK_SPEED = 10.0f;
}

TitlePlayer::TitlePlayer()
{

}

TitlePlayer::~TitlePlayer()
{
}

bool TitlePlayer::Start()
{
	m_titleCamera = FindGO<TitleCamera>("TitleCamera");

	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, TITLE_PLAYER_ANIMATION_OPTIONS, MODEL_PATH, MODEL_SCALE);

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	return true;
}

void TitlePlayer::Update()
{
	// シーン切り替え中は更新しない。
	if (SceneManager::GetInstance()->GetIsSceneChangeRequested()) {
		return;
	}

	if (m_titleCamera == nullptr)
	{
		return;
	}


	//「惑星の中心→キャラ」のベクトルを更新します。
	UpdateUpDirection();

	m_modelRender.PlayAnimation(enAnimationClip_Walk);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void TitlePlayer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

//void TitlePlayer::CalcFrontAndRightDirection()
//{
//	Vector3 moveDirection = ComputeMoveDirection();
//	// 正面方向を計算。
//	m_frontDirection = moveDirection;
//	// 右方向を計算。
//	m_rightDirection = Cross(m_upDirection, moveDirection);
//	m_rightDirection.Normalize();
//}

//const Vector3 TitlePlayer::ComputeMoveDirection() const
//{
//	// スティックの入力を取得。
//	Vector3 stickL = MOVE_DIRECTION;
//	stickL.Normalize();
//
//	// カメラの向きから正面を取得。
//	Vector3 forward = Vector3::Zero;
//	forward = g_camera3D->GetForward();
//	forward = ProjectOnPlane(forward, m_upDirection);
//	forward.Normalize();
//
//	// カメラの向きから右を取得。
//	Vector3 right = Vector3::Zero;
//	right = g_camera3D->GetRight();
//	right = ProjectOnPlane(right, m_upDirection);
//	right.Normalize();
//
//	// 方向設定
//	Vector3 direction = Vector3::Front;
//	direction = forward * stickL.y + right * stickL.x;
//	direction.Normalize();
//
//	return direction;
//}
