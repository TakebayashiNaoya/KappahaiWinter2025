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
	constexpr float BODY_COLLIDER_RADIUS = 50.0f;					// ゴーストオブジェクトの半径。
	constexpr float BODY_COLLIDER_OFFSET = 50.0f;					// ゴーストオブジェクトのオフセット値。

	constexpr float RUN_SPEED = 8.0f;								// 走る速度

	// 初期値が設定できず、プレイヤーがうつ伏せになってしまう問題を回避するため、Y座標を2000.1fに設定。
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 0.0f, 2000.0f);	// スポーン座標。
}

BasicEnemy::BasicEnemy()
{
	m_stateMachine = std::make_unique<app::basicEnemy::BasicEnemyStateMachine>(this);
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
	InitModel(enAnimationClip_Num, BASIC_ENEMY_ANIMATION_OPTIONS, "Wolf/wolf");
	m_modelRender.SetScale(Vector3(70.0f, 70.0f, 70.0f));

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// 初期ステートを設定
	m_stateMachine->InitializeState(enBasicEnemyState_Idle);

	// ゴーストオブジェクトを作成。
	m_bodyCollider = new CollisionObject();
	m_bodyCollider->CreateSphere(
		m_position,
		m_rotation,
		BODY_COLLIDER_RADIUS
	);

	// コリジョンヒットマネージャーに登録。
	CollisionHitManager::GetInstance()->Register(enCollisionType_BasicEnemy, m_bodyCollider, this);

	return true;
}

void BasicEnemy::Update()
{
	m_moveSpeed = Vector3::Zero;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	UpdateUpDirection();

	m_stateMachine->Update();

	UpdateBodyCollider(BODY_COLLIDER_OFFSET);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void BasicEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


