#include "stdafx.h"
#include "Player.h"
#include "PlayerStateMachine.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption Player::PLAYER_ANIMATION_OPTIONS[] = {
   {"idle",	true},
   {"walk", true},
   {"run",	true},
   {"jump", false}
};

namespace
{
	constexpr float GHOST_OBJECT_RADIUS = 25.0f;					// ゴーストオブジェクトの半径。
	constexpr float GHOST_OBJECT_HEIGHT = 75.0f;					// ゴーストオブジェクトの高さ。
	constexpr float GHOST_OBJECT_OFFSET = 60.0f;					// ゴーストオブジェクトのオフセット値。

	constexpr float GRAVITY_POWER = 9.8f;		// 重力。
	constexpr float DEADZONE = 0.01f;				// スティック入力検知の基準値。

	// 初期値が設定できず、プレイヤーがうつ伏せになってしまう問題を回避するため、Y座標を2000.1fに設定。
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 2000.1f, 0.0f);	// スポーン座標。

	constexpr int LIFE = 3;											// 初期ライフ数。
}

Player::Player()
{
	m_stateMachine = std::make_unique<app::player::PlayerStateMachine>(this);
}

/// <summary>
/// カメラの回転角度を計算します。
/// </summary>
void Player::CalcCameraRotation()
{
	if (m_moveSpeed.Length() <= DEADZONE) {
		return;
	}

	// 前方向
	Vector3 forwardDirection = m_moveSpeed;
	forwardDirection.Normalize();

	// 上方向(重力の逆)
	Vector3 upDirection = m_directionFromPlanetCenter;

	// XZ平面上の右方向ベクトルを計算
	Vector3 xzDirection;
	xzDirection.Cross(upDirection, forwardDirection);
	xzDirection.Normalize();

	// 回転の角度を求める。
	// この時点ではcosの範囲（-1.0～1.0）で算出されるため、後でacosを使って角度に変換する。
	float dotResult = m_directionFromPlanetCenter.Dot(m_beforeDirectionFromPlanetCenter);

	// acosの引数の範囲は-1.0f～1.0fだが、floatの誤差で範囲外の値が入ってしまうことがあるためクランプする。
	if (dotResult < -1.0f) {
		dotResult = -1.0f;
	}
	else if (dotResult > 1.0f) {
		dotResult = 1.0f;
	}

	// cosの値から角度に変換。
	float m_rotationAngle = acosf(dotResult);

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


bool Player::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, PLAYER_ANIMATION_OPTIONS, "unityChan");

	InitLife(LIFE);

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// 初期ステートを設定
	m_stateMachine->InitializeState(enPlayerState_Idle);

	// ゴーストオブジェクトを作成。
	m_ghostObject = new PhysicsGhostObject();
	m_ghostObject->CreateCapsule(
		m_position,
		m_rotation,
		GHOST_OBJECT_RADIUS,
		GHOST_OBJECT_HEIGHT
	);

	return true;
}

void Player::Update()
{
	m_beforeDirectionFromPlanetCenter = m_directionFromPlanetCenter;
	m_moveSpeed = Vector3::Zero;
	m_xzAdditionalRot = Quaternion::Identity;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	CalcDirectionFromPlanetCenter();

	m_stateMachine->Update();

	GhostObjectUpdate(GHOST_OBJECT_OFFSET);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

/// <summary>
/// 移動方向を返します。
/// </summary>
/// <returns>移動方向。</returns>
const Vector3 Player::ComputeMoveDirection() const
{
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

	return direction;
}

/// <summary>
/// 移動方向に速度を乗算して返します。
/// </summary>
/// <param name="speed"> 移動速度。</param>
/// <returns> 移動先の相対座標。</returns>
const Vector3 Player::ComputeAddMoveSpeed(const float speed) const
{
	Vector3 computeSpeed = ComputeMoveDirection() * speed;
	return computeSpeed;
}

/// <summary>
/// 入力方向、速度、重力に基づいてキャラクターを移動させます。
/// </summary>
/// <param name="speed">移動速度を表す値。</param>
void Player::MoveUpdate(const float speed)
{
	//////////////////////////
	///--- 平面移動処理 ---///
	//////////////////////////

	Vector3 move = ComputeAddMoveSpeed(speed);
	m_moveSpeed += move;						// 移動速度に加算。



	////////////////////////////////
	///--- ジャンプ・重力処理 ---///
	////////////////////////////////

	// 空中の移動速度の計算。
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	m_moveSpeed += m_directionFromPlanetCenter * jumpPower;	// 垂直方向に加算。



	//////////////////////
	///--- 移動処理 ---///
	//////////////////////

	Vector3 rayStartPos = m_position + m_directionFromPlanetCenter * 10.0f;	// 少し上からレイを飛ばす。
	Vector3 rayEndPos = m_position + m_moveSpeed;							// 移動先までレイを飛ばす。

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




	//
	//
	//
	//
	//	// XZ平面は移動量がないならできない
	//	if (move.LengthSq() >= 0.01f && m_initialJumpSpeed <= 0.0f) {
	//		// 平面上の移動処理をしてみる
	//		{
	//			// 平面に障害物がないかレイを飛ばして確認
	//			bool isHitXZ = true;
	//			{
	//				// TODO: 背の高い障害物などを出そうとしたときに処理を書いて
	//				//const Vector3 rayStartPos = m_position;
	//				//const Vector3 rayEndPos = m_position + m_moveSpeed;
	//				//Vector3 hitPos = Vector3::Zero;
	//				//isHitXZ = PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPos);
	//				//// isHit == true なら障害物がある
	//				//if (isHit) {
	//				//	move = hitPos - m_position;
	//				//}
	//			}
	//			// 地面が球体なので下方向にレイを飛ばして地面の上にする
	//			Vector3 movedPosition = m_position + m_moveSpeed;
	//			{
	//				Vector3 upVector = movedPosition - m_planetCenter;
	//				upVector.Normalize();
	//				Vector3 downVector = m_planetCenter - movedPosition;	// プレイヤーから見て下方向のベクトルを計算
	//				downVector.Normalize();
	//
	//				// 開始位置(ちょっと上から)
	//				Vector3 rayStartPos = movedPosition;			// 横移動
	//				rayStartPos += upVector * (isHitXZ ? 10.0f : 1.0f);	// 少し上にずらす(レイが球体にぴったりならヒットしないので)
	//				// 終了位置(ちょっと下)
	//				Vector3 rayEndPos = movedPosition;
	//				rayEndPos += downVector * (isHitXZ ? 15.0f : 1.0f);	// TODO: 要調整
	//				// isTrueなら地面があるので、地面の位置にする
	//				Vector3 hitPos;
	//				const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPos);
	//				if (isHit) {
	//					movedPosition = hitPos;
	//				}
	//			}
	//			m_position = movedPosition;
	//		}
	//	}
	//
	//	// 平面移動は確定できた
	//
	//	// ジャンプ処理
	//	{
	//		// ジャンプしていない
	//		if (m_initialJumpSpeed <= 0.0f) {
	//			return;
	//		}
	//
	//		// 空中の移動速度の計算。
	//		// 落下時間を加算
	//		m_fallTimer += g_gameTime->GetFrameDeltaTime();
	//
	//		// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//		float jumpPower = 2.0f - (GRAVITY_POWER * m_fallTimer);
	//		currentJumpPower += jumpPower;
	//
	//		// プレイヤーから見て下方向のベクトルを計算
	//		Vector3 upVector = m_position - m_planetCenter;
	//		upVector.Normalize();
	//
	//		Vector3 moveSpeed = Vector3::Zero;
	//		moveSpeed = upVector * currentJumpPower;
	//
	//		// 落下しているはず
	//		if (jumpPower < 0.0f) {
	//			// 落下中はレイで地面にぶつかるかを確認する
	//			Vector3 rayStartPos = m_position;
	//			Vector3 rayEndPos = m_position + moveSpeed;
	//			Vector3 hitPos = Vector3::Zero;
	//			if (PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPos)) {
	//				// 地面にぶつかった
	//				m_position = hitPos;
	//				// ジャンプ終了
	//				m_initialJumpSpeed = 0.0f;
	//				m_fallTimer = 0.0f;
	//				currentJumpPower = 0.0f;
	//				return;
	//			}
	//		}
	//
	//		m_position += moveSpeed;
	//	}
	//
	//}
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	// 
	//
	/////@TODO: レイが当たる前提の処理なので、落下時の座標が計算できない。
	//
	///// <summary>
	///// キャラクターを指定された速度で移動させます。
	///// </summary>
	///// <param name="speed">移動速度。</param>
	//void Player::MoveOnGround(const float speed)
	//{
	//	// 平面の移動速度の計算。
	//	{
	//		// スティックの入力を取得。
	//		Vector3 stickL = Vector3::Zero;
	//		stickL.x = g_pad[0]->GetLStickXF();
	//		stickL.y = g_pad[0]->GetLStickYF();
	//
	//		// カメラの向きから正面を取得。
	//		Vector3 forward = Vector3::Zero;
	//		forward = g_camera3D->GetForward();
	//		forward = ProjectOnPlane(forward, m_directionFromPlanetCenter);
	//		forward.Normalize();
	//
	//		// カメラの向きから右を取得。
	//		Vector3 right = Vector3::Zero;
	//		right = g_camera3D->GetRight();
	//		right = ProjectOnPlane(right, m_directionFromPlanetCenter);
	//		right.Normalize();
	//
	//		// 方向設定
	//		Vector3 direction = Vector3::Front;
	//		direction = forward * stickL.y + right * stickL.x;
	//		direction.Normalize();
	//
	//		// 入力方向（接線）を合成
	//		Vector3 wish = direction * speed;
	//
	//		// 速度に加算
	//		m_moveSpeed += wish;
	//	}
	//
	//	// 移動先が地面と近い場合は、地面に沿って移動する。
	//	// 次の移動地点を先に計算
	//	Vector3 rayStartPos = m_position + m_moveSpeed;
	//	// レイのスタート地点を少しだけ上にずらす。
	//	rayStartPos += m_directionFromPlanetCenter * 10.0f;
	//
	//	// 惑星の中心からレイのスタート地点への向きを計算。
	//	Vector3 directionToRayStartPos = rayStartPos - m_planetCenter;
	//	directionToRayStartPos.Normalize();
	//	// レイのエンド地点を地中に埋まる程度に下げる。
	//	Vector3 rayEndPos = rayStartPos - directionToRayStartPos * 20.0f;
	//
	//	// レイが当たった場合は地上を走っているとみなす。
	//	Vector3 hitPosition = Vector3::Zero;
	//	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, rayEndPos, hitPosition);
	//	if (isHit) {
	//		m_position = hitPosition;
	//		//return;
	//	}
	//
	//
	//
	//	// 空中の移動速度の計算。
	//	// 落下時間を加算
	//	m_fallTimer += g_gameTime->GetFrameDeltaTime();
	//
	//	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//	float upSpeed = m_initialJumpSpeed - GRAVITY_POWER * m_fallTimer;
	//
	//	// 移動速度に鉛直方向の速度を加算。
	//	Vector3 nextPos = m_position + m_moveSpeed + directionToRayStartPos * upSpeed;
	//	Vector3 rayStartPos2 = m_position + m_directionFromPlanetCenter * 10.0f;
	//
	//	Vector3 hitPosition2 = Vector3::Zero;
	//	const bool isHit2 = PhysicsWorld::GetInstance()->RayTest(rayStartPos2, nextPos, hitPosition2);
	//	if (isHit2) {
	//		m_position = hitPosition2;
	//		return;
	//	}
	//	else {
	//		m_position = nextPos;
	//	}
	//
	//	//// 次の移動地点をレイのスタート地点にして、惑星の中心に向かってレイを飛ばす。
	//	//// そして、レイが当たった位置を次の移動地点とする。
	//	//Vector3 hitPosition = Vector3::Zero;
	//	//const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, m_planetCenter, hitPosition);
	//	//if (isHit) {
	//	//	m_position = hitPosition;
	//	//}
	//}
	//
	//void Player::MoveOffGround()
	//{
	//	// 落下時間を加算
	//	m_fallTimer += g_gameTime->GetFrameDeltaTime();
	//
	//	// 現在の「惑星の中心→キャラ」の向きを取得
	//	Vector3 currentDirectionFromPlanetCenter = m_position - m_planetCenter;
	//	currentDirectionFromPlanetCenter.Normalize();
	//
	//	// 鉛直投げ上げ運動の公式を使ってy座標の変化量を計算。
	//	float y = (m_initialJumpSpeed * m_fallTimer) - (GRAVITY_POWER * m_fallTimer * m_fallTimer / 2);
	//
	//	// プレイヤーの上方向にy座標の変化量を加算。
	//	Vector3 jumpFallDisplacement = currentDirectionFromPlanetCenter * y;
	//
	//	// 座標と変化量を合成して、最終的な座標を計算。
	//	Vector3 nextPosition = m_position + jumpFallDisplacement;
	//
	//	// ゴーストオブジェクトにレイが当たらないように、オフセットを加える。
	//	Vector3 rayStartPos = m_position;
	//	Vector3 nextCheckPosition = m_planetCenter;
	//	// 上昇中はゴーストオブジェクトの上でレイを飛ばす。
	//	if (y >= 0.0f) {
	//		rayStartPos = m_position + currentDirectionFromPlanetCenter * 150.0f;
	//		nextCheckPosition = m_position + jumpFallDisplacement + currentDirectionFromPlanetCenter * 150.0f;
	//	}
	//	// 落下中はゴーストオブジェクトの下でレイを飛ばす。
	//	else {
	//		rayStartPos = m_position - currentDirectionFromPlanetCenter * 1.0f;
	//		nextCheckPosition = m_position + jumpFallDisplacement - currentDirectionFromPlanetCenter * 1.0f;
	//	}
	//
	//	Vector3 hitPosition = Vector3::Zero;
	//	const bool isHit = PhysicsWorld::GetInstance()->RayTest(rayStartPos, nextCheckPosition, hitPosition);
	//	// レイが当たっていたら、その位置を次の移動地点とする。
	//	if (isHit) {
	//		if (y > 0.0f) {
	//			return;
	//		}
	//		m_position = hitPosition;
	//	}
	//	// レイが当たっていなかったら、そのまま次の移動地点に移動。
	//	else {
	//		m_position = nextPosition;
	//	}
	//}

}