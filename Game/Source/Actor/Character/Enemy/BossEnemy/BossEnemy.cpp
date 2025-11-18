#include "stdafx.h"
#include "BossEnemy.h"
#include "BossEnemyStateMachine.h"
#include "Source/Collision/CollisionManager.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption BossEnemy::BOSS_ENEMY_ANIMATION_OPTIONS[] = {
   {"Bear/idle",	true},
   {"Bear/walk",	true},
   {"Bear/run",		true},
   {"Bear/attack",	false},
   {"Bear/damage",	false},
   {"Bear/dead",	false},
};

namespace
{
	const std::string MODEL_PATH = "Bear/bear";
	constexpr float MODEL_SCALE = 200.0f;

	const Vector3 BODY_COLLIDER_SIZE = Vector3(300.0f, 300.0f, 400.0f);	// ボディコライダーのサイズ。
	constexpr float BODY_COLLIDER_OFFSET = 100.0f;						// ボディコライダーのオフセット値。

	const Vector3 SPAWN_POSITION = Vector3(0.0f, 0.0f, 2000.0f);		// スポーン座標。

	constexpr float RUN_SPEED = 8.0f;									// 走る速度

	constexpr float COOLDOWN_DURATION = 1.0f;							// 攻撃のクールダウン時間。
}

BossEnemy::BossEnemy()
{
	m_stateMachine = std::make_unique<app::bossEnemy::BossEnemyStateMachine>(this);
}


/// <summary>
/// プレイヤーに向かって走ります。
/// </summary>
void BossEnemy::ChasePlayer()
{
	// 水平方向に速度加算。
	m_moveSpeed += CalcHorizontalVelocity(RUN_SPEED);

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
}

void BossEnemy::UpdateCooldown()
{
	m_cooldownTimer -= g_gameTime->GetFrameDeltaTime();
}

const bool BossEnemy::IsOnCooldown()const
{
	return m_cooldownTimer < 0.0f;
}

const void BossEnemy::ResetCooldownTimer()
{
	m_cooldownTimer = COOLDOWN_DURATION;
}

const float BossEnemy::GetDistanceToPlayer()const
{
	Vector3 distance = m_playerFoundPos - m_position;
	return distance.Length();
}

bool BossEnemy::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, BOSS_ENEMY_ANIMATION_OPTIONS, MODEL_PATH, MODEL_SCALE);

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// 初期ステートを設定
	m_stateMachine->InitializeState(enBossEnemyState_Idle);

	// やられ判定のコライダーを作成。
	m_hurtCollider = CollisionHitManager::GetInstance()->CreateCollider(this, enCollisionType_BossEnemy, BODY_COLLIDER_OFFSET);

	return true;
}

void BossEnemy::Update()
{
	m_moveSpeed = Vector3::Zero;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	UpdateUpDirection();

	m_stateMachine->Update();

	CollisionHitManager::GetInstance()->UpdateCollider(this, m_hurtCollider, BODY_COLLIDER_OFFSET);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void BossEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
