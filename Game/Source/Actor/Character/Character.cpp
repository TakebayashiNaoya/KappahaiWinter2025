#include "stdafx.h"
#include "Character.h"

// �ÓI�����o�̒�`
const std::string Character::ANIMATION_FILE_PATH = "Assets/animData/";
const std::string Character::ANIMATION_EXTENSION = ".tka";

namespace
{
	constexpr float GRAVITY_POWER = 10.0f;				// �d�́B
	constexpr float INPUT_DEADZONE_THRESHOLD = 0.01f;	// �X�e�B�b�N���͌��m�̊�l�B
	constexpr float STICK_ACCEL = 2.0f;					// �X�e�B�b�N���͂ɂ����������t���̋����B
}

/// <summary>
/// �w�肳�ꂽ�A�j���[�V�����ԍ��̃A�j���[�V�������Đ����܂��B
/// </summary>
/// <param name="animNo">�Đ�����A�j���[�V�����̔ԍ��B</param>
void Character::PlayAnimation(const int animNo)
{
	m_modelRender.PlayAnimation(animNo);
}

/// <summary>
/// �W�����v�p���[���W�����v�X�s�[�h�ƃ��[�u�X�s�[�h�ɑ����ɓK�p���܂��B
/// </summary>
/// <param name="jumpPower">�W�����v�p���[�B</param>
void Character::ApplyJumpImpulse(const float jumpPower)
{
	m_jumpSpeed = jumpPower;
	m_moveSpeed.y = m_jumpSpeed;
}

/// <summary>
/// �L�����N�^�[���w�肳�ꂽ���x�ňړ������܂��B
/// </summary>
/// <param name="speed">�ړ����x�B</param>
void Character::Move(float speed)
{
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	Vector3 stickL = Vector3::Zero;
	Vector3 forward = Vector3::Zero;
	Vector3 right = Vector3::Zero;

	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	forward = g_camera3D->GetForward();
	right = g_camera3D->GetRight();

	forward = ProjectOnPlane(forward, m_directionFromPlanetCenter);
	right = ProjectOnPlane(right, m_directionFromPlanetCenter);
	if (forward.LengthSq() > 1e-6f) {
		forward.Normalize();
	}
	if (right.LengthSq() > 1e-6f) {
		right.Normalize();
	}

	// ���͕����i�ڐ��j������
	Vector3 wish = right * (stickL.x * speed) + forward * (stickL.y * speed);

	// ���x�ɉ��Z
	m_moveSpeed += wish;


	Vector3 hitPosition = Vector3::Zero;
	const bool isHit = PhysicsWorld::GetInstance()->RayTest(m_position, m_planetCenter, hitPosition);
	if (isHit) {
		m_position = hitPosition;
	}

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	//forward.y = 0.0f;
	//right.y = 0.0f;

	//right *= stickL.x * speed;
	//forward *= stickL.y * speed;

	//m_moveSpeed += right + forward;
}

/// <summary>
/// moveSpeed�Ɋ�Â���Y����]���X�V���܂��B
/// </summary>
void Character::Rotation()
{
	if (fabsf(m_moveSpeed.x) >= INPUT_DEADZONE_THRESHOLD || fabsf(m_moveSpeed.z) >= INPUT_DEADZONE_THRESHOLD) {
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		m_modelRender.SetRotation(m_rotation);
	}


	//// �i�s������ڐ��֓��e
	//Vector3 fwd = ProjectOnPlane(m_moveSpeed, m_directionFromPlanetCenter);
	//if (fwd.LengthSq() < 1e-6f) {
	//	// �قڒ�~�F�J����Forward��ڐ��ɓ��e���Ďg�p
	//	fwd = ProjectOnPlane(g_camera3D->GetForward(), m_directionFromPlanetCenter);
	//}
	//fwd.Normalize();

	//// �E��n���\�z �� �I���\���K��
	//Vector3 right = Cross(fwd, m_directionFromPlanetCenter);
	//right.Normalize();
	//fwd = Cross(m_directionFromPlanetCenter, right);
	//fwd.Normalize();


	//// �� Matrix�Łu�� = right/up/fwd�v�����
	//Matrix basis;
	//basis._11 = right.x;	basis._12 = m_directionFromPlanetCenter.x;	basis._13 = fwd.x;	basis._14 = 0.0f;
	//basis._21 = right.y;	basis._22 = m_directionFromPlanetCenter.y;	basis._23 = fwd.y;  basis._24 = 0.0f;
	//basis._31 = right.z;	basis._32 = m_directionFromPlanetCenter.z;	basis._33 = fwd.z;  basis._34 = 0.0f;
	//basis._41 = 0.0f;		basis._42 = 0.0f;							basis._43 = 0.0f;	basis._44 = 1.0f;

	//// �s�� �� quat�iDirectXMath�j
	//DirectX::XMVECTOR dq = DirectX::XMQuaternionRotationMatrix((DirectX::XMMATRIX)basis);
	//DirectX::XMFLOAT4 qf;
	//DirectX::XMStoreFloat4(&qf, dq);
	//Quaternion target(qf.x, qf.y, qf.z, qf.w);

	//// ��Ԃ��ēK�p
	//m_rotation.Slerp(0.15f, m_rotation, target);
	//m_modelRender.SetRotation(m_rotation);
}

/// <summary>
/// �L�����N�^�[�̃��f���ƃA�j���[�V�����N���b�v�����������܂��B
/// </summary>
/// <param name="count">�A�j���[�V�����N���b�v�̐��B</param>
/// <param name="option">�e�A�j���[�V�����N���b�v�̐ݒ��񂪊i�[���ꂽAnimationOption�^�̔z��B</param>
void Character::InitModel(const size_t count, const AnimationOption* option)
{
	// �|�C���^�ɔz���new����ƁA�A���Ŋm�ۂ����B
	m_animationClips = new AnimationClip[count];

	// �A�j���[�V�����ǂݍ���
	for (size_t i = 0; i < count; i++) {
		m_animationClips[i].Load(option[i].GetFullPath().c_str());
		m_animationClips[i].SetLoopFlag(option[i].is_loop);
	}

	// ���f���̏������B
	m_modelRender.Init("Assets/modelData/Character/unityChan.tkm", m_animationClips, count, enModelUpAxisY);
}

/// <summary>
/// �d�͂�������B
/// </summary>
void Character::AddGravity()
{
	//�u�L�������f���̒��S�v�̃x�N�g�����v�Z���A���K�����܂��B
	CalcDirectionToPlanetCenter();

	//�u�f���̒��S���L�����v�̃x�N�g�����v�Z���A���K�����܂��B
	CalcDirectionFromPlanetCenter();

	//if (m_characterController.IsOnGround() == false) {
	//	m_jumpSpeed -= GRAVITY_POWER;
	//	m_moveSpeed += m_directionFromPlanetCenter * m_jumpSpeed;
	//}
	//else
	{
		m_jumpSpeed = 0.0f;
	}
}

/// <summary>
/// �u�L�������f���̒��S�v�̃x�N�g�����v�Z���A���K�����܂��B
/// </summary>
void Character::CalcDirectionToPlanetCenter()
{
	m_directionToPlanetCenter = m_planetCenter - m_position;
	m_directionToPlanetCenter.Normalize();
}

/// <summary>
/// �u�f���̒��S���L�����v�̃x�N�g�����v�Z���A���K�����܂��B
/// </summary>
void Character::CalcDirectionFromPlanetCenter()
{
	m_directionFromPlanetCenter = m_position - m_planetCenter;
	m_directionFromPlanetCenter.Normalize();
}
