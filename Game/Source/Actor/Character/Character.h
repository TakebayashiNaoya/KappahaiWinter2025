/// <summary>
/// �L�����N�^�[�̊��N���X�B
/// </summary>
#pragma once

class Character :public IGameObject
{
public:
	/// <summary>
	/// �L�����N�^�[�R���g���[���[�ւ̎Q�Ƃ��擾���܂��B
	/// </summary>
	/// <returns>�L�����N�^�[�R���g���[���[�iCharacterController�j�ւ̒萔�Q�ƁB</returns>
	const CharacterController& GetCharacterController() const
	{
		return m_characterController;
	}

	/// <summary>
	/// �w�肳�ꂽ�A�j���[�V�����ԍ��̃A�j���[�V�������Đ����܂��B
	/// </summary>
	/// <param name="animNo">�Đ�����A�j���[�V�����̔ԍ��B</param>
	void PlayAnimation(const int animNo);

	/// <summary>
	///�@�W�����v�p���[���W�����v�X�s�[�h�ƃ��[�u�X�s�[�h�ɑ����ɓK�p���܂��B
	/// </summary>
	/// <param name="jumpPower">�W�����v�p���[�B</param>
	void ApplyJumpImpulse(const float jumpPower);

	/// <summary>
	/// �L�����N�^�[���w�肳�ꂽ���x�ňړ������܂��B
	/// </summary>
	/// <param name="speed">�ړ����x�B</param>
	void Move(float speed);

	/// <summary>
	/// moveSpeed�Ɋ�Â���Y����]���X�V���܂��B
	/// </summary>
	void Rotation();

protected:
	ModelRender m_modelRender;							// ���f�������_�[�B
	Vector3 m_position = Vector3::Zero;					// �|�W�V�����B
	Quaternion m_rotation;								// ��]�B
	Vector3 m_moveSpeed = Vector3::Zero;				// �ړ����x�B
	AnimationClip* m_animationClips;					// �A�j���[�V�����N���b�v�B
	CharacterController m_characterController;			// �L�����N�^�[�R���g���[���[�B
	float m_jumpSpeed = 0.0f;							// �W�����v�̑����B
	Vector3 m_planetCenter = Vector3::Zero;				// �d�͂𓭂�����f���̒��S���W�B
	Vector3 m_directionToPlanetCenter = Vector3::Zero;	// ��������f���̒��S�ւ̕����x�N�g���B
	Vector3 m_directionFromPlanetCenter = Vector3::Zero;// �f���̒��S���玩���ւ̕����x�N�g���B

	// �ÓI�����o�Ƃ��Đ錾
	// �p�����.cpp�Œ�`���邱�ƁB
	static const std::string ANIMATION_FILE_PATH;
	static const std::string ANIMATION_EXTENSION;

	/// <summary>
	/// �A�j���[�V�����t�@�C���̃I�v�V�������Ǘ�����\���́B
	/// �p�����.cpp�Œ�`���邱�ƁB
	/// </summary>
	struct AnimationOption
	{
		const char* fileName;	// �t�@�C���p�X�B
		bool is_loop = false;	// ���s�[�g���邩�ǂ����B�itrue=����Afalse=���Ȃ��j

		/// <summary>
		/// �A�j���[�V�����t�@�C���̃t���p�X���擾���܂��B
		/// </summary>
		std::string GetFullPath() const
		{
			return ANIMATION_FILE_PATH + fileName + ANIMATION_EXTENSION;
		}
	};

	/// <summary>
	/// �L�����N�^�[�̃��f���ƃA�j���[�V�����N���b�v�����������܂��B
	/// </summary>
	/// <param name="count">�A�j���[�V�����N���b�v�̐��B</param>
	/// <param name="option">�e�A�j���[�V�����N���b�v�̐ݒ��񂪊i�[���ꂽAnimationOption�^�̔z��B</param>
	void InitModel(const size_t count, const AnimationOption* option);

	/// <summary>
	/// �d�͂�K�p���܂��B
	/// </summary>
	void AddGravity();

	/// <summary>
	/// �u�L�������f���̒��S�v�̃x�N�g�����v�Z���A���K�����܂��B
	/// </summary>
	void CalcDirectionToPlanetCenter();

	/// <summary>
	/// �u�f���̒��S���L�����v�̃x�N�g�����v�Z���A���K�����܂��B
	/// </summary>
	void CalcDirectionFromPlanetCenter();

	// �x�N�g�� v ��@�� n �̐ڕ��ʂ֓��e�i�ڐ����������o���j
	// Dot(v, n) �� v �� n �̓��� �� v �̒��� n �����ɂǂꂾ�����������邩�B
	// n * Dot(v, n) �͂��̐����� n �����ɖ߂����x�N�g���B
	// v - (���̐���) �� n�����̐����������āA�c���Ԃ� �� ���ʂ� n �ɒ������镽�ʏ�̃x�N�g���i�ڐ��j
	static Vector3 ProjectOnPlane(const Vector3& v, const Vector3& n)
	{
		return v - n * Dot(v, n);
	}
};

