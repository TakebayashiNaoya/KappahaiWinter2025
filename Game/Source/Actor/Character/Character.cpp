#include "stdafx.h"
#include "Character.h"

// 静的メンバの定義
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 9.8f * 100;		// 重力。
	constexpr float MAX_FALL_SPEED = 9.8f * 10;		// 最大落下速度。
	constexpr float DEADZONE = 0.01f;				// スティック入力検知の基準値。
	constexpr float STICK_ACCEL = 2.0f;				// スティック入力による微小押し付けの強さ。
}

/// <summary>
/// 地面に向かってレイを飛ばし、
/// 当たった座標と自分の座標の距離が一定未満、
/// あるいはレイが当たらなけらば接地していると判定します。
/// </summary>
const bool& Character::IsOnGround()
{
	// 移動処理でhitPositionをm_positionに代入しており、レイの判定が不安定になるため、rayStartをm_positionから少し離す。
	Vector3 rayStart = m_position + m_directionFromPlanetCenter * 0.1f;
	Vector3 hitPosition = Vector3::Zero;

	if (PhysicsWorld::GetInstance()->RayTest(rayStart, m_planetCenter, hitPosition)) {
		// レイが当たっていれば最後に当たった座標を記録。
		m_lastHitPosition = hitPosition;

		// キャラクター座標と当たった座標の距離を計算。
		Vector3 DistanceToGround = m_position - hitPosition;
		// 距離が一定未満なら接地していると判定。
		if (DistanceToGround.Length() < 0.1f) {
			return true;
		}

		return false;
	}
	// レイが当たっていない場合は地面下にいると判定し、最後にレイが当たった座標に戻す。
	else {
		m_position = m_lastHitPosition;
		return true;
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
	m_jumpUpSpeed = jumpPower;
}

/// <summary>
/// キャラクターを指定された速度で移動させます。
/// </summary>
/// <param name="speed">移動速度。</param>
void Character::MoveOnGround(const float speed)
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
	forward.Normalize();

	// カメラの向きから右を取得。
	Vector3 right = Vector3::Zero;
	right = g_camera3D->GetRight();
	right = ProjectOnPlane(right, m_directionFromPlanetCenter);
	right.Normalize();

	// 方向設定
	Vector3 direction = Vector3::Front;
	direction = forward * stickL.y + right * stickL.x;
	direction.Normalize();

	// 入力方向（接線）を合成
	Vector3 wish = direction * speed;

	// 速度に加算
	m_moveSpeed += wish;

	// 次の移動地点を先に計算
	Vector3 rayStartPos = Vector3::Zero;
	rayStartPos = m_position + m_moveSpeed;
	// レイのスタート地点を少しだけ上にずらす。
	rayStartPos += m_directionFromPlanetCenter * 5.0f;

	// 次の移動地点をレイのスタート地点にして、惑星の中心に向かってレイを飛ばす。
	// そして、レイが当たった位置を次の移動地点とする。
	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}
}

void Character::MoveOffGround()
{
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 現在の「惑星の中心→キャラ」の向きを取得
	Vector3 currentDirectionFromPlanetCenter = m_position - m_planetCenter;
	currentDirectionFromPlanetCenter.Normalize();

	float y = (m_jumpUpSpeed * m_fallTimer) - (GRAVITY_POWER * m_fallTimer * m_fallTimer / 2);

	Vector3 airPos = currentDirectionFromPlanetCenter * y;

	m_position += airPos;

}

/// <summary>
/// moveSpeedに基づいてY軸回転を更新します。
/// </summary>
void Character::Rotation()
{
	if (fabsf(m_moveSpeed.x) <= DEADZONE && fabsf(m_moveSpeed.y) <= DEADZONE && fabsf(m_moveSpeed.z) <= DEADZONE) {
		return;
	}

	// カメラが追従するための回転角度を計算する。
	{
		// 前方向
		Vector3 forwardDirection = m_moveSpeed;
		forwardDirection.Normalize();
		// 上方向(重力の逆)
		Vector3 upDirection = m_directionFromPlanetCenter;

		Vector3 xzDirection;
		xzDirection.Cross(upDirection, forwardDirection);
		xzDirection.Normalize();

		// 回転の角度を求める
		float dotResult = m_directionFromPlanetCenter.Dot(m_beforeDirectionFromPlanetCenter);

		// acosの引数の範囲は-1.0f～1.0fなので、範囲外の値が入ってしまったら補正する
		if (dotResult < -1.0f) {
			dotResult = -1.0f;
		}
		else if (dotResult > 1.0f) {
			dotResult = 1.0f;
		}

		m_rotationAngle = acosf(dotResult);

		// 回転の向き（符号）を外積で判定する
		Vector3 m_rotationDirection = Vector3::Zero;
		m_rotationDirection.Cross(m_beforeDirectionFromPlanetCenter, m_directionFromPlanetCenter);

		// もし回転軸と外積の向きが逆なら、角度にマイナスをつける
		if (m_rotationDirection.Dot(xzDirection) < 0.0f) {
			m_rotationAngle *= -1.0f;
		}

		// 回転の角度を加算
		m_xzAdditionalRot.SetRotation(xzDirection, m_rotationAngle);
	}


	// 自身の回転処理
	{
		// キャラクターの新しい前方ベクトルを計算 (目標の移動方向)
		Vector3 targetForward = m_moveSpeed;
		targetForward.Normalize();

		// 惑星の中心からキャラクターへの上方向ベクトルを計算
		Vector3 upDirection = m_directionFromPlanetCenter;
		upDirection.Normalize();

		// モデルのデフォルトの上方向(0, 1, 0)を、惑星の上方向(upDirection)に回転させるクォータニオンを計算
		Quaternion planetAlignmentRotation;
		planetAlignmentRotation.SetRotation(Vector3::Up, upDirection);

		// 回転前のモデルの前方向(0, 0, 1)を、ターゲットの移動方向(targetForward)に回転させるクォータニオンを計算
		Quaternion movementRotation;
		// 惑星にアライメントされた状態で、モデルの前方向（Vector3::Front）がどこに向いているかを求める
		// これは、planetAlignmentRotationをVector3::Frontに適用することで得られる
		Vector3 currentForward = Vector3::Front;
		planetAlignmentRotation.Apply(currentForward); // これが惑星に沿った状態での「前」

		// currentForward（回転後の前）をtargetForwardに回転させるためのクォータニオンを求める
		// ただし、回転軸はupDirection（キャラクターの真上）に限定する必要がある

		// 回転軸を計算: 上方向
		Vector3 rotationAxis = upDirection;

		// 回転角度を計算: currentForwardとtargetForwardの間の角度
		Vector3 projectedCurrentForward = currentForward;
		projectedCurrentForward.Normalize();
		Vector3 projectedTargetForward = targetForward;
		projectedTargetForward.Normalize();

		float dotResult = projectedCurrentForward.Dot(projectedTargetForward);
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
		crossProduct.Cross(projectedCurrentForward, projectedTargetForward);

		if (crossProduct.Dot(rotationAxis) < 0.0f) {
			rotationAngle *= -1.0f;
		}

		// Y軸周りの回転クォータニオンを作成。
		Quaternion yRotation;
		yRotation.SetRotation(rotationAxis, rotationAngle);

		// 「惑星アライメント」と「Y軸回転」を乗算。
		Quaternion targetRotation = yRotation * planetAlignmentRotation;

		m_rotation = targetRotation;
	}

	m_modelRender.SetRotation(m_rotation);
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
/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
/// </summary>
void Character::CalcDirectionFromPlanetCenter()
{
	m_beforeDirectionFromPlanetCenter = m_directionFromPlanetCenter;
	m_directionFromPlanetCenter = m_position - m_planetCenter;
	m_directionFromPlanetCenter.Normalize();
}
