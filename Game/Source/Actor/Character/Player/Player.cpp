#include "stdafx.h"
#include "Player.h"
#include "PlayerStateMachine.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption Player::PLAYER_ANIMATION_OPTIONS[] = {
   {"idle",	true},
   {"walk", true},
   {"run",	true},
   {"jump", false},
   {"kneelDown", false}
};

namespace
{
	// プレイヤーのヒットボックスは小さめに設定。
	constexpr float BODY_COLLIDER_RADIUS = 15.0f;					// ボディコライダーの半径。
	constexpr float BODY_COLLIDER_HEIGHT = 60.0f;					// ボディコライダーの高さ。
	constexpr float BODY_COLLIDER_OFFSET = 50.0f;					// ボディコライダーのオフセット値。

	constexpr float STOMP_COLLIDER_RADIUS = 25.0f;					// 踏みつけ用コライダーの半径。
	constexpr float STOMP_JUMP_POWER = 30.0f;						// 踏みつけジャンプの初速。

	constexpr float GRAVITY_POWER = 9.8f * 10;						// 重力。
	constexpr float DEADZONE = 0.01f;								// スティック入力検知の基準値。

	constexpr float INITIAL_KNOCK_BACK_SPEED = 10.0f;				// ノックバック初速。
	constexpr float KNOCK_BACK_DAMPING = 10;						// ノックバック減衰率。
	constexpr float INVINCIBLE_TIME = 5.0f;							// 無敵時間。

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
	Vector3 upDirection = m_upDirection;

	// XZ平面上の右方向ベクトルを計算
	Vector3 xzDirection;
	xzDirection.Cross(upDirection, forwardDirection);
	xzDirection.Normalize();

	// 回転の角度を求める。
	// この時点ではcosの範囲（-1.0～1.0）で算出されるため、後でacosを使って角度に変換する。
	float dotResult = m_upDirection.Dot(m_beforeDirectionFromPlanetCenter);

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
	m_rotationDirection.Cross(m_beforeDirectionFromPlanetCenter, m_upDirection);

	// もし回転軸と外積の向きが逆なら、角度にマイナスをつける
	if (m_rotationDirection.Dot(xzDirection) < 0.0f) {
		m_rotationAngle *= -1.0f;
	}

	// 回転の角度を加算
	m_xzAdditionalRot.SetRotation(xzDirection, m_rotationAngle);
}

/// <summary>
/// 入力方向、速度、重力に基づいてキャラクターを移動させます。
/// </summary>
/// <param name="speed">移動速度を表す値。</param>
void Player::MoveUpdate(const float speed)
{
	///--- 平面移動処理 ---///
	Vector3 move = CalcVelocity(speed);
	m_moveSpeed += move;				// 移動速度に加算。


	///--- ジャンプ・重力処理 ---///
	// 空中の移動速度の計算。
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	m_moveSpeed += m_upDirection * jumpPower;	// 垂直方向に加算。


	///--- 移動処理 ---///
	Vector3 rayStartPos = m_position + m_upDirection * 10.0f;	// 少し上からレイを飛ばす。
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
}

/// <summary>
/// 踏みつけ判定用ゴーストオブジェクトを作成します。
/// </summary>
void Player::CreateStompCollider()
{
	m_stompCollider = new CollisionObject();
	m_stompCollider->CreateSphere(
		m_position,
		m_rotation,
		STOMP_COLLIDER_RADIUS
	);
}

/// <summary>
/// 踏みつけ判定用ゴーストオブジェクトの座標と回転を更新します。
/// </summary>
void Player::UpdateStompCollider()
{
	m_stompCollider->SetPosition(m_position);
	m_stompCollider->SetRotation(m_rotation);
}

/// <summary>
/// 踏みつけ判定用コライダーをdelete、nullptrします。
/// </summary>
void Player::DeleteStompCollider()
{
	delete m_stompCollider;
	m_stompCollider = nullptr;
}

/// <summary>
/// 攻撃してきた敵の方向を計算します。
/// </summary>
/// <param name="enemyPos"> 敵の座標。</param>
void Player::ComputeAttackedDirection(const Vector3& enemyPos)
{
	// 敵の位置から「惑星の中心→敵」のベクトルを計算し、正規化します。
	Vector3 directionToEnemy = enemyPos - m_position;
	directionToEnemy.Normalize();

	// 攻撃された方向を計算します。
	m_attackedDirection = ProjectOnPlane(directionToEnemy, m_upDirection);
}

/// <summary>
/// ノックバックされる処理を実行する関数。
/// </summary>
void Player::KnockedBack()
{
	m_knockedbackTimer += g_gameTime->GetFrameDeltaTime();

	// ノックバック速度を計算。
	float knockedBackSpeed = INITIAL_KNOCK_BACK_SPEED - (KNOCK_BACK_DAMPING * m_knockedbackTimer);

	// ノックバック速度が0以下になったら処理を終了。
	if (knockedBackSpeed < 0.0f)
	{
		m_isAttacked = false;
		return;
	}

	Vector3 knockBackDirection = m_attackedDirection * -1.0f;

	// ノックバック速度を移動速度に加算。
	m_moveSpeed += knockBackDirection * knockedBackSpeed;

	///--- ジャンプ・重力処理 ---///
	// 空中の移動速度の計算。
	// 落下時間を加算
	m_fallTimer += g_gameTime->GetFrameDeltaTime();

	// 鉛直投げ上げ運動の公式を使って鉛直方向の速度を計算。
	//速度 = 初速度 - 重力 * 時間。
	float jumpPower = m_initialJumpSpeed - (GRAVITY_POWER * m_fallTimer);

	m_moveSpeed += m_upDirection * jumpPower;	// 垂直方向に加算。


	///--- 移動処理 ---///
	Vector3 rayStartPos = m_position + m_upDirection * 10.0f;	// 少し上からレイを飛ばす。
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
}

/// <summary>
/// エネミーを踏みつけたときのジャンプ処理。
/// </summary>
void Player::StompJump()
{
	// 移動速度をリセット。
	m_moveSpeed = Vector3::Zero;
	// ジャンプ初速度を設定。
	m_initialJumpSpeed = STOMP_JUMP_POWER;
	// 落下タイマーをリセット。
	m_fallTimer = 0.0f;
}

/// <summary>
/// ライフを1減らします。
/// </summary>
void Player::TakeDamage()
{
	m_life--;
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

	// ボディのゴーストオブジェクトを作成。
	m_bodyCollider = new CollisionObject();
	m_bodyCollider->CreateCapsule(
		m_position,
		m_rotation,
		BODY_COLLIDER_RADIUS,
		BODY_COLLIDER_HEIGHT
	);
	return true;
}

void Player::Update()
{
	m_beforeDirectionFromPlanetCenter = m_upDirection;
	m_moveSpeed = Vector3::Zero;
	m_xzAdditionalRot = Quaternion::Identity;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	CalcDirectionFromPlanetCenter();

	m_stateMachine->Update();

	UpdateBodyCollider(BODY_COLLIDER_OFFSET);

	InvincibleTimer();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	// 無敵時間中は点滅処理を行う。
	if (m_isInvincible) {
		m_isBlinking = !m_isBlinking;
		if (m_isBlinking) {
			m_modelRender.Draw(rc);
		}
	}
	else {
		m_modelRender.Draw(rc);
	}
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
	forward = ProjectOnPlane(forward, m_upDirection);
	forward.Normalize();

	// カメラの向きから右を取得。
	Vector3 right = Vector3::Zero;
	right = g_camera3D->GetRight();
	right = ProjectOnPlane(right, m_upDirection);
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
const Vector3 Player::CalcVelocity(const float speed) const
{
	Vector3 computeSpeed = ComputeMoveDirection() * speed;
	return computeSpeed;
}

/// <summary>
/// 一定時間が経過したら無敵状態を解除します。
/// </summary>
void Player::InvincibleTimer()
{
	m_invincibleTimer += g_gameTime->GetFrameDeltaTime();
	if (m_invincibleTimer >= INVINCIBLE_TIME) {
		m_isInvincible = false;
		m_invincibleTimer = 0.0f;
	}
}