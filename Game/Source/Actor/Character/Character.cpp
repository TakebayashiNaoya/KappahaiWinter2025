#include "stdafx.h"
#include "Character.h"
#include "Source/Collision/CollisionManager.h"

// 静的メンバの定義
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 9.8f * 10;		// 重力。
	constexpr float DEADZONE = 0.01f;				// スティック入力検知の基準値。
	constexpr float STICK_ACCEL = 2.0f;				// スティック入力による微小押し付けの強さ。

	const std::string MODEL_FILE_PATH = "Assets/modelData/Character/";
	const std::string MODEL_EXTENSION = ".tkm";
}

/// <summary>
/// 地面に向かってレイを飛ばし、
/// 当たった座標と自分の座標の距離が一定未満、
/// あるいはレイが当たらなけらば接地していると判定します。
/// </summary>
const bool& Character::IsOnGround()
{
	// 移動処理でhitPositionをm_positionに代入しており、レイの判定が不安定になるため、rayStartをm_positionから少し離す。
	Vector3 rayStart = m_position + m_upDirection * 0.1f;
	Vector3 hitPosition = Vector3::Zero;

	if (PhysicsWorld::GetInstance()->RayTest(rayStart, m_planetCenter, hitPosition)) {
		// レイが当たっていれば最後に当たった座標を記録。
		m_lastHitPosition = hitPosition;

		// キャラクター座標と当たった座標の距離を計算。
		Vector3 DistanceToGround = m_position - hitPosition;
		// 距離が一定未満なら接地していると判定。
		if (DistanceToGround.Length() < 2.0f) {
			return true;
		}

		return false;
	}
	// レイが当たっていない場合は地面下にいると判定し、最後にレイが当たった座標に戻す。
	else {
		//m_position = m_lastHitPosition;
		//return true;
	}

	return false;
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
/// ジャンプの勢い（jumpPower）を適用し、内部のジャンプ速度を設定します。
/// </summary>
/// <param name="jumpPower">適用するジャンプの強さ（速度）。この値を内部の m_jumpSpeed に設定します。</param>
void Character::ApplyJumpImpulse(const float jumpPower)
{
	m_initialJumpSpeed = jumpPower;
}

/// <summary>
/// moveSpeedに基づいてモデルを回転させます。
/// </summary>
void Character::ModelRotation()
{
	// 惑星の中心からキャラクターへの上方向ベクトルを計算
	Vector3 upDirection = m_upDirection;
	upDirection.Normalize();

	// m_moveSpeedを惑星の接平面に投影し、ジャンプによる垂直成分を除去する。
	Vector3 forward = ProjectOnPlane(m_moveSpeed, upDirection);

	//////////////////////////////////////////////////////////////////////////////////////////////
	///--- 停止時・moveSpeedが真上を向いているときに現在のキャラクターの向きを維持する処理。---///
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (forward.Length() < DEADZONE) {
		//// 現在の回転から、ワールド空間での「前方向」を取得
		//Vector3 currentForward = Vector3::Front;
		//m_rotation.Apply(currentForward);
		//// それを現在の接平面に投影しなおす（惑星表面を移動して法線が変わっている可能性があるため）
		//forward = ProjectOnPlane(currentForward, upDirection);

		// 長さが0の場合は回転できないので、そのまま返す。
		return;
	}

	forward.Normalize();

	// モデルのデフォルトの上方向(0, 1, 0)を、惑星の上方向(upDirection)に回転させるクォータニオンを計算
	Quaternion planetAlignmentRotation;
	planetAlignmentRotation.SetRotation(Vector3::Up, upDirection);

	// 惑星にアライメントされた状態で、モデルの前方向（Vector3::Front）がどこに向いているかを求める
	Vector3 projectedDefaultForward = Vector3::Front;
	planetAlignmentRotation.Apply(projectedDefaultForward); // これが惑星に沿った状態での「前」
	projectedDefaultForward.Normalize();

	// キャラクターが今向いている方向。
	Vector3 projectedTargetForward = forward;
	projectedTargetForward.Normalize();

	// 惑星に沿った状態のデフォルトの前方向から、moveSpeedの方向への回転角度を求める。
	float dotResult = projectedDefaultForward.Dot(projectedTargetForward);
	// acosの引数をクランプ
	if (dotResult < -1.0f) {
		dotResult = -1.0f;
	}
	else if (dotResult > 1.0f) {
		dotResult = 1.0f;
	}
	float rotationAngle = acosf(dotResult);

	// 回転の向き（符号）を外積で判定。
	Vector3 crossProduct = Vector3::Zero;
	crossProduct.Cross(projectedDefaultForward, projectedTargetForward);
	if (crossProduct.Dot(upDirection) < 0.0f) {
		rotationAngle *= -1.0f;
	}

	// Y軸周りの回転クォータニオンを作成。
	Quaternion yRotation;
	yRotation.SetRotation(upDirection, rotationAngle);

	// 「惑星アライメント」と「Y軸回転」を乗算。
	Quaternion targetRotation = yRotation * planetAlignmentRotation;

	// 球面線形補間(Slerp)を入れると、回転が滑らかになる。
	// m_rotation.Slerp(m_rotation, targetRotation, 0.2f); 
	m_rotation = targetRotation;

	m_modelRender.SetRotation(m_rotation);
}

/// <summary>
/// 現在の座標に合わせて、強制的にキャラクターを惑星に対して直立させます。
/// </summary>
void Character::ResetRotation()
{
	UpdateUpDirection();

	Quaternion planetAlignment;
	planetAlignment.SetRotation(Vector3::Up, m_upDirection);

	m_rotation = planetAlignment;
	m_modelRender.SetRotation(m_rotation);
}



/// <summary>
/// キャラクターのモデルとアニメーションクリップを初期化します。
/// </summary>
/// <param name="count">アニメーションクリップの数。</param>
/// <param name="option">各アニメーションクリップの設定情報が格納されたAnimationOption型の配列。</param>
/// <param name="path">モデルファイルのパス。</param>
/// <param name="scale">モデルの拡大率。（規定値は1.0f）</param>
void Character::InitModel(const size_t count, const AnimationOption* option, const std::string path, const float scale)
{
	// ポインタに配列でnewすると、連続で確保される。
	m_animationClips = new AnimationClip[count];

	// アニメーション読み込み
	for (size_t i = 0; i < count; i++) {
		m_animationClips[i].Load(option[i].GetFullPath().c_str());
		m_animationClips[i].SetLoopFlag(option[i].is_loop);
	}

	// モデルの初期化。
	std::string fullModelPath = MODEL_FILE_PATH + path + MODEL_EXTENSION;
	m_modelRender.Init(fullModelPath.c_str(), m_animationClips, count, enModelUpAxisY);

	// モデルの拡大率を設定。
	m_modelRender.SetScale(scale, scale, scale);
}

/// <summary>
/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
/// </summary>
void Character::UpdateUpDirection()
{
	m_beforeUpDirection = m_upDirection;
	m_upDirection = m_position - m_planetCenter;
	m_upDirection.Normalize();
}

/// <summary>
/// ジャンプや重力から、垂直方向の速度ベクトルを計算して返します。
/// </summary>
/// <returns> 垂直方向の速度。/returns>
const Vector3 Character::CalcVerticalVelocity()
{
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	// 垂直方向に加算。
	Vector3 velocity = m_upDirection * jumpPower;

	return velocity;
}

/// <summary>
/// 移動速度から移動後の座標を計算します。
/// </summary>
void Character::ComputePosition()
{
	Vector3 rayStartPos = m_position + m_upDirection * 10.0f;	// 少し上からレイを飛ばす。
	Vector3 rayEndPos = m_position + m_moveSpeed;				// 移動先までレイを飛ばす。

	// レイが地面に当たったら、その位置に移動させる。
	Vector3 hitPos = Vector3::Zero;
	if (PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPos)) {
		// 地面にぶつかった
		m_position = hitPos;
		// ジャンプ終了
		m_initialJumpSpeed = 0.0f;
		m_fallTimer = 0.0f;
		return;
	}
	// 地面にぶつからなかったら、そのまま移動させる。
	m_position = rayEndPos;
}
