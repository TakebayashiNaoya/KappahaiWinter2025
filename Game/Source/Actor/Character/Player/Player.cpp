#include "stdafx.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Player/PlayerStateMachine.h"

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

	// 初期値が設定できず、プレイヤーがうつ伏せになってしまう問題を回避するため、Y座標を2000.1fに設定。
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 2000.1f, 0.0f);	// スポーン座標。

	constexpr int LIFE = 3;											// 初期ライフ数。
}

Player::Player()
{
	m_stateMachine = std::make_unique<app::player::PlayerStateMachine>(this);
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