#include "stdafx.h"
#include "Source/Actor/Character/Player/Player.h"
#include "Source/Actor/Character/Player/PlayerStateMachine.h"

// �w�b�_�[��static�錾�������A������R���X�g���N�^�Œ�`����΁A�����N���X���g���Ă��ႤPLAYER_ANIMATION_OPTIONS��ݒ�ł���B
// �����Astatic�̕��������������͗ǂ��̂ō���͂��̌`�B
const Character::AnimationOption Player::PLAYER_ANIMATION_OPTIONS[] = {
   {"idle", true},
   {"walk", true},
   {"run", true},
   {"jump", false}
};

namespace
{
	constexpr float CHARACTER_CONTROLLER_SCALE_RADIUS = 25.0f;		// �L�����N�^�[�R���g���[���[�̔��a�B
	constexpr float CHARACTER_CONTROLLER_SCALE_HEIGHT = 75.0f;		// �L�����N�^�[�R���g���[���[�̍����B
	const Vector3 SPAWN_POSITION = Vector3(0.0f, 2000.0f, 0.0f);	// �X�|�[�����W�B
}

Player::Player()
{
	m_stateMachine = std::make_unique<app::player::PlayerStateMachine>(this);
}

bool Player::Start()
{
	// ���f���ƃA�j���[�V�������������B
	InitModel(enAnimationClip_Num, PLAYER_ANIMATION_OPTIONS);

	// ���ɖ�����Ȃ��悤�ɏ����ʒu�𒲐��B
	m_position = SPAWN_POSITION;

	// �L�����N�^�[�R���g���[���[���������B
	m_characterController.Init(CHARACTER_CONTROLLER_SCALE_RADIUS, CHARACTER_CONTROLLER_SCALE_HEIGHT, m_position);

	// �����X�e�[�g��ݒ�
	m_stateMachine->InitializeState(enPlayerState_Idle);

	return true;
}

void Player::Update()
{
	m_moveSpeed = Vector3::Zero;

	// �����t���Ă��邩�̔�������̃t���[���ōs���������߁Am_stateMachine->Update();�̏�ɏ����Ă���B
	AddGravity();

	m_stateMachine->Update();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}