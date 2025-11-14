#include "stdafx.h"
#include "DeformEnemy.h"
#include "DeformEnemyStateMachine.h"
#include "Source/Collision/CollisionManager.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption DeformEnemy::TRANSFORM_ENEMY_ANIMATION_OPTIONS[] = {
   {"Spider/idle",	true},
   {"Spider/walk",	true},
   {"Spider/run",	true},
   {"Spider/dead",	false},
};

namespace
{
	constexpr float BODY_COLLIDER_RADIUS = 40.0f;					// ゴーストオブジェクトの半径。
	constexpr float BODY_COLLIDER_OFFSET = 30.0f;					// ゴーストオブジェクトのオフセット値。

	constexpr float RUN_SPEED = 3.0f;								// 走る速度
	constexpr float SLIDE_SPEED = 16.0f;							// 滑走速度

	// 初期値が設定できず、プレイヤーがうつ伏せになってしまう問題を回避するため、Y座標を2000.1fに設定。
	const Vector3 SPAWN_POSITION = Vector3(100.0f, 0.0f, 2000.0f);	// スポーン座標。
}

DeformEnemy::DeformEnemy()
{
	m_stateMachine = std::make_unique<app::deformEnemy::DeformEnemyStateMachine>(this);
}

DeformEnemy::~DeformEnemy()
{
	if (m_bodyCollider)
	{
		DeleteBodyCollider();
	}
}

/// <summary>
/// プレイヤーが一定範囲内にいる場合、プレイヤーから逃げる。
/// </summary>
void DeformEnemy::EscapePlayer()
{
	// 水平方向に速度加算。
	m_moveSpeed += CalcHorizontalVelocity(RUN_SPEED);

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
}

/// <summary>
/// 滑走方向を更新します。
/// </summary>
void DeformEnemy::UpdateSlideDirection()
{
	m_slideDirection = ProjectOnPlane(m_slideDirection, m_upDirection);
	m_slideDirection.Normalize();
}

/// <summary>
/// 滑走します。
/// </summary>
void DeformEnemy::Sliding()
{
	// 滑走方向を再計算。
	UpdateSlideDirection();

	// 水平方向に速度加算。
	m_moveSpeed += m_slideDirection * SLIDE_SPEED;

	// 垂直方向に速度加算。
	m_moveSpeed += CalcVerticalVelocity();

	// 移動速度から座標更新。
	ComputePosition();
}

void DeformEnemy::DeleteEnemy()
{
	// コライダーを削除。
	DeleteBodyCollider();
	// 自分自身を削除。
	DeleteGO(this);
}

bool DeformEnemy::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, TRANSFORM_ENEMY_ANIMATION_OPTIONS, "Spider/spider");
	m_modelRender.SetScale(Vector3(30.0f, 30.0f, 30.0f));

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// 初期ステートを設定
	m_stateMachine->InitializeState(enDeformEnemyState_Idle);

	// ゴーストオブジェクトを作成。
	m_bodyCollider = new CollisionObject();
	m_bodyCollider->CreateSphere(
		m_position,
		m_rotation,
		BODY_COLLIDER_RADIUS
	);

	// コリジョンヒットマネージャーに登録。
	CollisionHitManager::GetInstance()->Register(enCollisionType_TransformEnemy, m_bodyCollider, this);

	return true;
}

void DeformEnemy::Update()
{
	m_moveSpeed = Vector3::Zero;

	//「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	UpdateUpDirection();

	m_stateMachine->Update();

	UpdateBodyCollider(BODY_COLLIDER_OFFSET);

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void DeformEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

/// <summary>
/// プレイヤーから逃げる方向を計算して返します。
/// </summary>
/// <returns> 逃走方向。</returns>
const Vector3 DeformEnemy::ComputeMoveDirection() const
{
	// プレイヤーからの方向ベクトルを計算。
	Vector3 directionFromPlayer = m_position - m_playerFoundPos;
	directionFromPlayer.Normalize();

	// プレイヤーからの方向ベクトルから、接線方向を取得。
	Vector3 moveDirection = ProjectOnPlane(directionFromPlayer, m_upDirection);
	moveDirection.Normalize();

	return moveDirection;
}
