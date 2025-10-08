/// <summary>
/// ステージ（惑星）の基底クラス。
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
	/// モデル、座標、静的物理オブジェクトの初期化を行います。
	/// </summary>
	void InitModel(const std::string& filePath, const Vector3& position);

	ModelRender m_modelRender;					// モデルレンダー。
	std::string m_filePath;						// モデルのファイル名。
	Vector3 m_position = Vector3::Zero;			// モデルの座標。
	PhysicsStaticObject physicsStaticObject;	// モデルの当たり判定。
};

/// <summary>
/// 最初の惑星を管理するクラス。
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