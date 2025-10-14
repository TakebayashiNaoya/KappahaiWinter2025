#include "stdafx.h"
#include "Character.h"

// 静的メンバの定義
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 10.0f;				// 重力。
	constexpr float INPUT_DEADZONE_THRESHOLD = 0.01f;	// スティック入力検知の基準値。
	constexpr float STICK_ACCEL = 2.0f;					// スティック入力による微小押し付けの強さ。
}

/// <summary>
/// 指定されたアニメーション番号のアニメーションを再生します。
/// </summary>
/// <param name="animNo">再生するアニメーションの番号。</param>
void Character::PlayAnimation(const int animNo)
{
	m_modelRender.PlayAnimation(animNo);
}

/// <summary>
/// ジャンプパワーをジャンプスピードとムーブスピードに即座に適用します。
/// </summary>
/// <param name="jumpPower">ジャンプパワー。</param>
void Character::ApplyJumpImpulse(const float jumpPower)
{
	m_jumpSpeed = jumpPower;
	m_moveSpeed.y = m_jumpSpeed;
}

/// <summary>
/// キャラクターを指定された速度で移動させます。
/// </summary>
/// <param name="speed">移動速度。</param>
void Character::Move(float speed)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	// スティックの入力を取得。
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	// カメラの向きから正面を取得。
	Vector3 forward = Vector3::Zero;
	forward = g_camera3D->GetForward();
	forward = ProjectOnPlane(forward, m_directionFromPlanetCenter);
	//if (forward.LengthSq() > 1e-6f) {
	forward.Normalize();
	//}

	// カメラの向きから右を取得。
	Vector3 right = Vector3::Zero;
	right = g_camera3D->GetRight();
	right = ProjectOnPlane(right, m_directionFromPlanetCenter);
	//if (right.LengthSq() > 1e-6f) {
	right.Normalize();
	//}

	// 入力方向（接線）を合成
	Vector3 wish = right * (stickL.x * speed) + forward * (stickL.y * speed);

	// 速度に加算
	m_moveSpeed += wish;

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(m_position, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}



	//forward.y = 0.0f;
	//right.y = 0.0f;

	//right *= stickL.x * speed;
	//forward *= stickL.y * speed;

	//m_moveSpeed += right + forward;
}

/// <summary>
/// moveSpeedに基づいてY軸回転を更新します。
/// </summary>
void Character::Rotation()
{
	if (fabsf(m_moveSpeed.x) >= INPUT_DEADZONE_THRESHOLD || fabsf(m_moveSpeed.z) >= INPUT_DEADZONE_THRESHOLD) {
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//m_rotation.SetRotation(m_directionFromPlanetCenter, atan2f(m_moveSpeed.x, m_moveSpeed.z));
		m_modelRender.SetRotation(m_rotation);
	}
}

/// <summary>
/// キャラクターのモデルとアニメーションクリップを初期化します。
/// </summary>
/// <param name="count">アニメーションクリップの数。</param>
/// <param name="option">各アニメーションクリップの設定情報が格納されたAnimationOption型の配列。</param>
void Character::InitModel(const size_t count, const AnimationOption* option)
{
	// ポインタに配列でnewすると、連続で確保される。
	m_animationClips = new AnimationClip[count];

	// アニメーション読み込み
	for (size_t i = 0; i < count; i++) {
		m_animationClips[i].Load(option[i].GetFullPath().c_str());
		m_animationClips[i].SetLoopFlag(option[i].is_loop);
	}

	// モデルの初期化。
	m_modelRender.Init("Assets/modelData/Character/unityChan.tkm", m_animationClips, count, enModelUpAxisY);
}

/// <summary>
/// 重力を加える。
/// </summary>
void Character::AddGravity()
{
	//「キャラ→惑星の中心」のベクトルを計算し、正規化します。
	CalcDirectionToPlanetCenter();

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	CalcDirectionFromPlanetCenter();

	//if (m_characterController.IsOnGround() == false) {
	//	m_jumpSpeed -= GRAVITY_POWER;
	//	m_moveSpeed += m_directionFromPlanetCenter * m_jumpSpeed;
	//}
	//else
	{
		m_jumpSpeed = 0.0f;
	}
}

/// <summary>
/// 「キャラ→惑星の中心」のベクトルを計算し、正規化します。
/// </summary>
void Character::CalcDirectionToPlanetCenter()
{
	m_directionToPlanetCenter = m_planetCenter - m_position;
	m_directionToPlanetCenter.Normalize();
}

/// <summary>
/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
/// </summary>
void Character::CalcDirectionFromPlanetCenter()
{
	m_directionFromPlanetCenter = m_position - m_planetCenter;
	m_directionFromPlanetCenter.Normalize();
}
