#include "stdafx.h"
#include "Player.h"
#include "PlayerStateMachine.h"
#include "Source/Collision/CollisionManager.h"
#include "Source/Battle/BattleManager.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption Player::PLAYER_ANIMATION_OPTIONS[] = {
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

	// プレイヤーのヒットボックスは小さめに設定。
	constexpr float HURT_COLLIDER_RADIUS = 30.0f;					// ボディコライダーの半径。
	constexpr float HURT_COLLIDER_HEIGHT = 60.0f;					// ボディコライダーの高さ。
	constexpr float COLLIDER_OFFSET = 50.0f;					// ボディコライダーのオフセット値。

	constexpr float STOMP_JUMP_POWER = 30.0f;						// 踏みつけジャンプの初速。

	constexpr float GRAVITY_POWER = 9.8f * 10;						// 重力。
	constexpr float DEADZONE = 0.01f;								// スティック入力検知の基準値。

	constexpr float INITIAL_KNOCK_BACK_SPEED = 10.0f;				// ノックバック初速。
	constexpr float KNOCK_BACK_DAMPING = 10;						// ノックバック減衰率。
	constexpr float INVINCIBLE_TIME = 5.0f;							// 無敵時間。

	constexpr int LIFE = 3;											// 初期ライフ数。
}

Player::Player()
{
	m_stateMachine = std::make_unique<app::player::PlayerStateMachine>(this);
}

Player::~Player()
{
	m_hurtCollider = CollisionHitManager::DeleteCollider(m_hurtCollider);
	m_attackCollider = CollisionHitManager::DeleteCollider(m_attackCollider);
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
	float dotResult = m_upDirection.Dot(m_beforeUpDirection);

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
	m_rotationDirection.Cross(m_beforeUpDirection, m_upDirection);

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
	// 移動方向に速度加算。
	m_moveSpeed += CalcHorizontalVelocity(speed);

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
}


/// <summary>
/// ノックバック方向を計算します。
/// </summary>
/// <param name="enemyPos"> 攻撃してきた敵の座標。</param>
void Player::ComputeKnockBackDirection(const Vector3& enemyPos)
{
	// 敵の位置から「惑星の中心→敵」のベクトルを計算し、正規化します。
	Vector3 directionToEnemy = enemyPos - m_position;
	directionToEnemy.Normalize();

	// 攻撃された方向を計算します。
	Vector3 attackedDirection = ProjectOnPlane(directionToEnemy, m_upDirection);

	// ノックバック方向を設定します。（攻撃された方向の逆向き）
	m_knockBackDirection = attackedDirection * -1.0f;
	m_knockBackDirection.Normalize();
}

/// <summary>
/// ノックバックされる処理を実行する関数。
/// </summary>
void Player::KnockedBack()
{
	m_knockBackTimer += g_gameTime->GetFrameDeltaTime();

	// ノックバック速度を計算。
	float knockedBackSpeed = INITIAL_KNOCK_BACK_SPEED - (KNOCK_BACK_DAMPING * m_knockBackTimer);

	// ノックバック速度が0以下になったら処理を終了。
	if (knockedBackSpeed < 0.0f)
	{
		m_isAttacked = false;
		return;
	}

	// ノックバック速度を移動速度に加算。
	m_moveSpeed += m_knockBackDirection * knockedBackSpeed;

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
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


bool Player::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, PLAYER_ANIMATION_OPTIONS, MODEL_PATH, MODEL_SCALE);

	InitLife(LIFE);

	// 初期ステートを設定
	m_stateMachine->InitializeState(enPlayerState_Idle);

	if (CollisionHitManager::GetInstance())
	{
		// やられ判定のコライダーを作成。
		m_hurtCollider = CollisionHitManager::GetInstance()->CreateCollider(
			this,
			enCollisionType_Player,
			HURT_COLLIDER_RADIUS,
			true
		);
	}

	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void Player::Update()
{
	// ポーズ中または戦闘終了時は更新しない。
	if (BattleManager::IsBattleFinish()) {
		return;
	}

	if (BattleManager::IsBattleFinish()) {
		StopLoopSound();
		return;
	}

	m_moveSpeed = Vector3::Zero;
	m_xzAdditionalRot = Quaternion::Identity;

	//「惑星の中心→キャラ」のベクトルを更新します。
	UpdateUpDirection();

	m_stateMachine->Update();

	CollisionHitManager::GetInstance()->UpdateCollider(this, m_hurtCollider, COLLIDER_OFFSET);

	InvincibleTimer();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	if (m_life <= 0) {
		SetIsDying(true);
	}
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

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"first_step") == 0) {
		SoundManager::Play(enSoundList_PlayerStep1SE);
	}
	else if (wcscmp(eventName, L"second_step") == 0) {
		SoundManager::Play(enSoundList_PlayerStep2SE, false, true, m_position);
	}
}
