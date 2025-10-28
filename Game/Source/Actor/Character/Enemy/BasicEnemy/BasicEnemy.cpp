#include "stdafx.h"
#include "BasicEnemy.h"
#include "BasicEnemyStateMachine.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption BasicEnemy::BASIC_ENEMY_ANIMATION_OPTIONS[] = {
   {"idle",	true},
   {"walk", true},
   {"run",	true},
   {"jump", false}
};

namespace
{
	constexpr float BODY_COLLIDER_RADIUS = 25.0f;					// ゴーストオブジェクトの半径。
	constexpr float BODY_COLLIDER_HEIGHT = 75.0f;					// ゴーストオブジェクトの高さ。
	constexpr float BODY_COLLIDER_OFFSET = 60.0f;					// ゴーストオブジェクトのオフセット値。

	constexpr float GRAVITY_POWER = 9.8f;							// 重力。
	constexpr float DEADZONE = 0.01f;								// スティック入力検知の基準値。

	// 初期値が設定できず、プレイヤーがうつ伏せになってしまう問題を回避するため、Y座標を2000.1fに設定。
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 0.0f, 2001.0f);	// スポーン座標。
}

BasicEnemy::BasicEnemy()
{
	m_stateMachine = std::make_unique<app::basicEnemy::BasicEnemyStateMachine>(this);
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

/// <summary>
/// エネミーを消滅させます。
/// </summary>
void BasicEnemy::DeleteEnemy()
{
	DeleteBodyCollider();

	m_modelRender.SetScale(Vector3(1.0f, 0.5f, 1.0f));

	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	if (m_deleteTimer >= 1.0f) {
		DeleteGO(this);
	}
}

bool BasicEnemy::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, BASIC_ENEMY_ANIMATION_OPTIONS, "unityChan");

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// 初期ステートを設定
	m_stateMachine->InitializeState(enPlayerState_Idle);

	// ゴーストオブジェクトを作成。
	m_bodyCollider = new CollisionObject();
	m_bodyCollider->CreateCapsule(
		m_position,
		m_rotation,
		BODY_COLLIDER_RADIUS,
		BODY_COLLIDER_HEIGHT
	);

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
