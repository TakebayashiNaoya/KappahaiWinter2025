#include "stdafx.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Player/PlayerStateMachine.h"

// ヘッダーのstatic宣言を消し、これをコンストラクタで定義すれば、同じクラスを使っても違うPLAYER_ANIMATION_OPTIONSを設定できる。
// ただ、staticの方がメモリ効率は良いので今回はこの形。
const Character::AnimationOption Player::PLAYER_ANIMATION_OPTIONS[] = {
   {"idle", true},
   {"walk", true},
   {"run", true},
   {"jump", false}
};

namespace
{
	constexpr float CHARACTER_CONTROLLER_SCALE_RADIUS = 25.0f;		// キャラクターコントローラーの半径。
	constexpr float CHARACTER_CONTROLLER_SCALE_HEIGHT = 75.0f;		// キャラクターコントローラーの高さ。
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 2000.0f, 0.0f);	// スポーン座標。
}

Player::Player()
{
	m_stateMachine = std::make_unique<app::player::PlayerStateMachine>(this);
}

bool Player::Start()
{
	// モデルとアニメーションを初期化。
	InitModel(enAnimationClip_Num, PLAYER_ANIMATION_OPTIONS);

	// 星に埋もれないように初期位置を調整。
	m_position = SPAWN_POSITION;

	// キャラクターコントローラーを初期化。
	m_characterController.Init(CHARACTER_CONTROLLER_SCALE_RADIUS, CHARACTER_CONTROLLER_SCALE_HEIGHT, m_position);

	// 初期ステートを設定
	m_stateMachine->InitializeState(enPlayerState_Idle);

	return true;
}

void Player::Update()
{
	m_moveSpeed = Vector3::Zero;

	// 足が付いているかの判定を次のフレームで行いたいため、m_stateMachine->Update();の上に書いている。
	AddGravity();

	m_stateMachine->Update();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}