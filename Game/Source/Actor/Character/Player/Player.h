/// <summary>
/// �v���C���[���Ǘ�����N���X�B
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/Character.h"

namespace app {
	namespace player {
		class PlayerStateMachine;
	}
}

class Player :public Character
{
public:
	Player();

	/// <summary>
	/// �A�j���[�V�����̃N���b�v��\���񋓌^�ł��B
	/// PlayerStateMachine�ŃA�j����؂�ւ��邽�߂Ɏg�p���܂��B
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// �ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,	// �����A�j���[�V�����B
		enAnimationClip_Run,	// ����A�j���[�V�����B
		enAnimationClip_Jump,	// �W�����v�A�j���[�V�����B
		enAnimationClip_Num,
	};

	/// <summary>
	/// �v���C���[�̍��W���擾���܂��B
	/// </summary>
	/// <returns>�v���C���[�̌��݂̍��W�l�ifloat�^�j�B</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// �v���C���[�̃X�e�[�g�}�V���B�B
	std::unique_ptr<app::player::PlayerStateMachine> m_stateMachine;

	// �N���X�X�R�[�v�Ő錾���Acpp�Œ�`�B
	static const Character::AnimationOption PLAYER_ANIMATION_OPTIONS[];
};