/// <summary>
/// ステージ（惑星）の基底クラス。
/// </summary>
#pragma once
class StageBase : public IGameObject
{
public:
	StageBase();
	~StageBase();


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
	Quaternion m_rotation;						// 惑星の回転。
	PhysicsStaticObject physicsStaticObject;	// モデルの当たり判定。
};