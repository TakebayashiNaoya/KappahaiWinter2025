#include "stdafx.h"
#include "BasicEnemy.h"
#include "BasicEnemyStateMachine.h"
#include "Source/Collision/CollisionManager.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption BasicEnemy::BASIC_ENEMY_ANIMATION_OPTIONS[] = {
   {"Wolf/idle",	true},
   {"Wolf/walk", true},
   {"Wolf/run",	true},
};

namespace
{
	const std::string MODEL_PATH = "Wolf/wolf";
	constexpr float MODEL_SCALE = 70.0f;

	constexpr float HIT_COLLIDER_RADIUS = 50.0f;					// 当たりコライダーのサイズ。
	constexpr float HURT_COLLIDER_RADIUS = 100.0f;					// やられコライダーのサイズ。
	constexpr float COLLIDER_OFFSET = 50.0f;						// ゴーストオブジェクトのオフセット値。

	constexpr float RUN_SPEED = 8.0f;								// 走る速度
}

BasicEnemy::BasicEnemy()
{
	m_stateMachine = std::make_unique<app::basicEnemy::BasicEnemyStateMachine>(this);
}

BasicEnemy::~BasicEnemy()
{
}

/// <summary>
/// プレイヤーに向かって走ります。
/// </summary>
void BasicEnemy::ChasePlayer()
{
	// 水平方向に速度加算。
	m_moveSpeed += CalcHorizontalVelocity(RUN_SPEED);

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
}

/// <summary>
/// クールダウンをカウントダウンします。
/// </summary>
void BasicEnemy::CoolDownCount()
{
	m_coolDownTimer += g_gameTime->GetFrameDeltaTime();
	if (m_coolDownTimer >= 5.0f) {
		m_isCoolDown = false;
		m_coolDownTimer = 0.0f;
	}
}


bool BasicEnemy::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, BASIC_ENEMY_ANIMATION_OPTIONS, MODEL_PATH, MODEL_SCALE);

	// 初期ステートを設定
	m_stateMachine->InitializeState(enBasicEnemyState_Idle);

	// 攻撃判定のコライダーを作成。
	m_hitCollider = CollisionHitManager::GetInstance()->CreateCollider(
		this,
		enCollisionType_BasicEnemy,
		HIT_COLLIDER_RADIUS,
		app::EnCollisionAttr::enCollisionAttr_Enemy
	);

	// やられ判定のコライダーを作成。
	m_hurtCollider = CollisionHitManager::GetInstance()->CreateCollider(
		this,
		enCollisionType_BasicEnemy,
		HURT_COLLIDER_RADIUS,
		app::EnCollisionAttr::enCollisionAttr_Enemy
	);

	m_modelRender.Update();
	return true;
}

void BasicEnemy::Update()
{
	// ポーズ中または戦闘終了時は更新しない。
	if (BattleManager::GetIsBattleFinish()) {
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
}

void BasicEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


