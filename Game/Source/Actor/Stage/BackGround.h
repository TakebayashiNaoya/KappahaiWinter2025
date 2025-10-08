/// <summary>
/// �X�e�[�W�i�f���j�̊��N���X�B
/// </summary>
#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();

protected:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

	/// <summary>
	/// ���f���A���W�A�ÓI�����I�u�W�F�N�g�̏��������s���܂��B
	/// </summary>
	void InitModel(const std::string& filePath, const Vector3& position);

	ModelRender m_modelRender;					// ���f�������_�[�B
	std::string m_filePath;						// ���f���̃t�@�C�����B
	Vector3 m_position = Vector3::Zero;			// ���f���̍��W�B
	PhysicsStaticObject physicsStaticObject;	// ���f���̓����蔻��B
};

/// <summary>
/// �ŏ��̘f�����Ǘ�����N���X�B
/// </summary>
class FirstPlanet :public BackGround
{
public:
	FirstPlanet();
	~FirstPlanet();
private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;
};