#include "stdafx.h"
#include "Character.h"

// 静的メンバの定義
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 10.0f;		// 重力。
	constexpr float DEADZONE = 0.01f;			// スティック入力検知の基準値。
	constexpr float STICK_ACCEL = 2.0f;			// スティック入力による微小押し付けの強さ。


	// @todo for test
	static Vector3 stickDirection = Vector3::Zero;
	static Vector3 oldStickDirection = Vector3::Zero;

	static Vector3 moveDirection = Vector3::Zero;

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
/// キャラクターを指定された速度で移動させます。
/// </summary>
/// <param name="speed">移動速度。</param>
void Character::Move(const float speed)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	// スティックの入力を取得。
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	// @todo for test
	//if (stickL.LengthSq() >= 0.001f)
	{
		oldStickDirection = stickDirection;
		stickDirection = stickL;
	}

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
	Vector3 m_direction = Vector3::Front;
	m_direction = forward * stickL.y + right * stickL.x;
	m_direction.Normalize();

	// 入力方向（接線）を合成
	//Vector3 wish = m_direction * speed;
	Vector3 wish = m_direction * 10.0f;

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

	//Vector3 gravity = m_directionToPlanetCenter * GRAVITY_POWER;
	//m_position = m_characterController.Execute(gravity, 1.0f / 60.0f);
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


	// Y軸方向に回転させたい
	{
		//Vector3 moveDirection = m_moveSpeed;
		//Vector3 oldMoveDirection = m_beforeMoveSpeed;
		//moveDirection.Normalize();
		//oldMoveDirection.Normalize();

		//Vector3 upVector = m_directionFromPlanetCenter;
		//upVector.Normalize();


		//Vector3 afterXZ;
		//afterXZ.Cross(m_beforeDirectionFromPlanetCenter, m_moveSpeed);
		//afterXZ.Normalize();

		//Vector3 afterFront;
		//afterFront.Cross(afterXZ, m_beforeDirectionFromPlanetCenter);
		//afterFront.Normalize();

		//// 回転の角度を求める
		//float dotResult = oldMoveDirection.Dot(afterFront);
		//// acosの引数の範囲は-1.0f～1.0fなので、範囲外の値が入ってしまったら補正する
		//if (dotResult < -1.0f) {
		//	dotResult = -1.0f;
		//}
		//else if (dotResult > 1.0f) {
		//	dotResult = 1.0f;
		//}
		//m_rotationAngle = acosf(dotResult);

		//// 回転の角度を加算
		//Quaternion yRot;
		//yRot.SetRotation(m_directionFromPlanetCenter, m_rotationAngle);
		//m_rotation *= yRot;


		//const float dotResult = moveDirection.Dot(oldMoveDirection);
		//Vector3 cross = moveDirection;
		//cross.Cross(oldMoveDirection);
		//float computeRotation = acosf(dotResult);
		//if (cross.Dot(upVector) < 0.0f) {
		//	// 右回転
		//	computeRotation *= -1.0f;
		//}
		//else {
		//	// 左回転
		//}


		// @todo for test
		//Quaternion upRot;
		//upRot.SetRotation(upVector, computeRotation);
		//float newAngle = atan2(stickDirection.y, stickDirection.x);
		//float oldAngle = atan2(oldStickDirection.y, oldStickDirection.x);
		//upRot.SetRotation(upVector, newAngle - oldAngle);
		//m_rotation *= upRot;


		// 1. キャラクターの新しい前方ベクトルを計算 (目標の移動方向)
		Vector3 targetForward = m_moveSpeed;
		targetForward.Normalize();

		// 2. 惑星の中心からキャラクターへの上方向ベクトルを計算
		Vector3 upDirection = m_directionFromPlanetCenter;
		upDirection.Normalize();

		// --- 1. 惑星へのアライメント (SetRotation(from, to)を使用) ---
		// モデルのデフォルトの上方向(0, 1, 0)を、惑星の上方向(upDirection)に回転させるクォータニオンを計算
		Quaternion planetAlignmentRotation;
		planetAlignmentRotation.SetRotation(Vector3::Up, upDirection);

		// --- 2. 移動方向への回転 (SetRotation(from, to)を使用) ---
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
		if (dotResult < -1.0f) dotResult = -1.0f;
		if (dotResult > 1.0f) dotResult = 1.0f;
		float rotationAngle = acosf(dotResult);

		// 回転の向き（符号）を外積で判定
		Vector3 crossProduct = Vector3::Zero;
		crossProduct.Cross(projectedCurrentForward, projectedTargetForward);

		if (crossProduct.Dot(rotationAxis) < 0.0f) {
			rotationAngle *= -1.0f;
		}

		// Y軸周りの回転クォータニオンを作成
		Quaternion yRotation;
		yRotation.SetRotation(rotationAxis, rotationAngle);

		// --- 3. 最終的な回転を合成 ---
		// 最終的な目標回転は「惑星アライメント」と「Y軸回転」の組み合わせ
		Quaternion targetRotation = yRotation * planetAlignmentRotation;

		// 現在の回転と目標の回転を補間してスムーズな回転を実現（任意）
		// m_rotation.Slerp(0.2f, m_rotation, targetRotation); // Slerpを使う場合はこれ

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
