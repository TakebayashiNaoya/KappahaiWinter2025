

/// <summary>
/// ステージ（惑星）の基底クラス。
/// </summary>
#pragma once
#include "Source/Actor/Actor.h" 

class PlanetBase : public Actor
{
public:
	PlanetBase();
	~PlanetBase();


protected:
	virtual bool Start() override;
	virtual void Update() override;
	virtual void Render(RenderContext& rc) override;

	/// <summary>
	/// モデル、座標、静的物理オブジェクトの初期化を行います。
	/// </summary>
	void InitModel(const std::string filePath, const Vector3 position);

	std::string m_filePath;						// モデルのファイル名。
	PhysicsStaticObject physicsStaticObject;	// モデルの当たり判定。
};