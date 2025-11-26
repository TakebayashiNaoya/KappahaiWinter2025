#pragma once
class Actor : public IGameObject
{
public:
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns>座標ベクトル。</returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns>回転クォータニオン。</returns>
	const Quaternion GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// 拡大率を取得。
	/// </summary>
	/// <returns>拡大率ベクトル。</returns>
	const Vector3 GetScale() const
	{
		return m_scale;
	}

	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}
	void SetRotation(const Quaternion rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}
	void SetTRS(const Vector3 position, const Quaternion rotation, const Vector3 scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
	}


protected:
	Vector3 m_position = Vector3::Zero;	// 座標。
	Quaternion m_rotation = Quaternion::Identity;	// 回転。
	Vector3 m_scale = Vector3::One;		// 拡大率。
	ModelRender m_modelRender;		// モデルレンダラー。


public:
	Actor() {};
	virtual ~Actor() {};


protected:
	virtual bool Start() override { return true; };
	virtual void Update() override {};
	virtual void Render(RenderContext& rc) override {};
};

