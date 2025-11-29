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

	constexpr int MAX_LIFE = 10;										// 最大体力。

	constexpr float COLLIDER_OFFSET = 100.0f;							// ボディコライダーのオフセット値。
	constexpr float HIT_COLLIDER_RADIUS = 100.0f;						// 当たりコライダーのサイズ。
	constexpr float HURT_COLLIDER_RADIUS = 300.0f;						// やられコライダーのサイズ。

	const Vector3 SPAWN_POSITION = Vector3(0.0f, 0.0f, 2000.0f);		// スポーン座標。

	constexpr float COOLDOWN_DURATION = 3.0f;							// 攻撃のクールダウン時間。
	constexpr float ATTACK_RANGE = 300.0f;								// 攻撃範囲。
	constexpr float ATTACK_RADIUS = 200.0f;								// 攻撃判定の半径。
}

BossEnemy::BossEnemy()
{
	m_stateMachine = std::make_unique<app::bossEnemy::BossEnemyStateMachine>(this);
}


/// <summary>
/// プレイヤーに向かって走ります。
/// </summary>
void BossEnemy::ChasePlayer(const float speed)
{
	// 水平方向に速度加算。
	m_moveSpeed += CalcHorizontalVelocity(speed);

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();

	// 攻撃方向を設定。
	SetAttackDirection(m_moveSpeed);
}

void BossEnemy::UpdateCooldown()
{
	m_cooldownTimer -= g_gameTime->GetFrameDeltaTime();
}

const bool BossEnemy::IsOnCooldown()const
{
	return m_cooldownTimer > 0.0f;
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


void BossEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		if (CollisionHitManager::GetInstance() == nullptr) {
			return;
		}

		m_attackCollider = CollisionHitManager::GetInstance()->CreateCollider(
			this,
			enCollisionType_BossEnemy,
			ATTACK_RADIUS,
			true
		);

		Vector3 attackPosition = m_position + m_attackDirection * ATTACK_RANGE;
		m_attackCollider->SetPosition(attackPosition);
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		if (CollisionHitManager::GetInstance() == nullptr) {
			return;
		}

		//攻撃用コライダーを削除。
		m_attackCollider = CollisionHitManager::GetInstance()->DeleteCollider(m_attackCollider);
	}
}


bool BossEnemy::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, BOSS_ENEMY_ANIMATION_OPTIONS, MODEL_PATH, MODEL_SCALE);

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	ResetRotation();

	m_maxLife = MAX_LIFE;
	InitLife(m_maxLife);

	// 初期ステートを設定
	m_stateMachine->InitializeState(enBossEnemyState_Idle);

	// 攻撃判定のコライダーを作成。
	m_hitCollider = CollisionHitManager::GetInstance()->CreateCollider(
		this,
		enCollisionType_BossEnemy,
		HIT_COLLIDER_RADIUS,
		true
	);

	// やられ判定のコライダーを作成。
	m_hurtCollider = CollisionHitManager::GetInstance()->CreateCollider(
		this,
		enCollisionType_BossEnemy,
		HURT_COLLIDER_RADIUS,
		true
	);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void BossEnemy::Update()
{
	// ポーズ中または戦闘終了時は更新しない。
	if (m_isPause) {
		return;
	}
	if (m_isBattleFinish) {
		return;
	}

	m_moveSpeed = Vector3::Zero;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	UpdateUpDirection();

	m_stateMachine->Update();

	CollisionHitManager::GetInstance()->UpdateCollider(this, m_hitCollider, COLLIDER_OFFSET);
	CollisionHitManager::GetInstance()->UpdateCollider(this, m_hurtCollider, COLLIDER_OFFSET);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	if (m_life <= 0) {
		SetIsDying(true);
	}

}

void BossEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
