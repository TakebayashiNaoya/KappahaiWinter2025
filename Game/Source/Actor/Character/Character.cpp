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

	Vector3 stickL = Vector3::Zero;
	Vector3 forward = Vector3::Zero;
	Vector3 right = Vector3::Zero;

	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	forward = g_camera3D->GetForward();
	right = g_camera3D->GetRight();

	forward = ProjectOnPlane(forward, m_directionFromPlanetCenter);
	right = ProjectOnPlane(right, m_directionFromPlanetCenter);
	if (forward.LengthSq() > 1e-6f) {
		forward.Normalize();
	}
	if (right.LengthSq() > 1e-6f) {
		right.Normalize();
	}

	// 入力方向（接線）を合成
	Vector3 wish = right * (stickL.x * speed) + forward * (stickL.y * speed);

	// 速度に加算
	m_moveSpeed += wish;


	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(m_position, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

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
		m_modelRender.SetRotation(m_rotation);
	}


	//// 進行方向を接線へ投影
	//Vector3 fwd = ProjectOnPlane(m_moveSpeed, m_directionFromPlanetCenter);
	//if (fwd.LengthSq() < 1e-6f) {
	//	// ほぼ停止：カメラForwardを接線に投影して使用
	//	fwd = ProjectOnPlane(g_camera3D->GetForward(), m_directionFromPlanetCenter);
	//}
	//fwd.Normalize();

	//// 右手系を構築 → オルソ正規化
	//Vector3 right = Cross(fwd, m_directionFromPlanetCenter);
	//right.Normalize();
	//fwd = Cross(m_directionFromPlanetCenter, right);
	//fwd.Normalize();


	//// ★ Matrixで「列 = right/up/fwd」を作る
	//Matrix basis;
	//basis._11 = right.x;	basis._12 = m_directionFromPlanetCenter.x;	basis._13 = fwd.x;	basis._14 = 0.0f;
	//basis._21 = right.y;	basis._22 = m_directionFromPlanetCenter.y;	basis._23 = fwd.y;  basis._24 = 0.0f;
	//basis._31 = right.z;	basis._32 = m_directionFromPlanetCenter.z;	basis._33 = fwd.z;  basis._34 = 0.0f;
	//basis._41 = 0.0f;		basis._42 = 0.0f;							basis._43 = 0.0f;	basis._44 = 1.0f;

	//// 行列 → quat（DirectXMath）
	//DirectX::XMVECTOR dq = DirectX::XMQuaternionRotationMatrix((DirectX::XMMATRIX)basis);
	//DirectX::XMFLOAT4 qf;
	//DirectX::XMStoreFloat4(&qf, dq);
	//Quaternion target(qf.x, qf.y, qf.z, qf.w);

	//// 補間して適用
	//m_rotation.Slerp(0.15f, m_rotation, target);
	//m_modelRender.SetRotation(m_rotation);
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
