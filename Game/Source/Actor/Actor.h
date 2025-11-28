#pragma once
class Actor : public IGameObject
{
public:
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns> 座標ベクトル。</returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <returns> 回転クォータニオン。</returns>
	const Quaternion GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// 拡大率を取得。
	/// </summary>
	/// <returns> 拡大率ベクトル。</returns>
	const Vector3 GetScale() const
	{
		return m_scale;
	}
	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position"> 座標ベクトル。</param>
	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="rotation"> 回転クォータニオン。</param>
	void SetRotation(const Quaternion rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 拡大率を設定。
	/// </summary>
	/// <returns> 拡大率ベクトル。</returns>
	void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// トランスフォームを一括設定。
	/// </summary>
	/// <param name="position"> 座標ベクトル。</param>
	/// <param name="rotation"> 回転クォータニオン。</param>
	/// <param name="scale"> 拡大率ベクトル。</param>
	void SetTRS(const Vector3 position, const Quaternion rotation, const Vector3 scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
	}


	/// <summary>
	/// ポーズフラグを取得。
	/// </summary>
	/// <returns> ポーズフラグ。</returns>
	const bool GetIsPause()const
	{
		return m_isPause;
	}
	/// <summary>
	/// ポーズフラグを設定。
	/// </summary>
	/// <returns> ポーズフラグ。</returns>
	static void SetIsPause(bool isPause)
	{
		m_isPause = isPause;
	}
	/// <summary>
	/// 戦闘終了フラグを取得。
	/// </summary>
	/// <returns> 戦闘終了フラグ。</returns>
	const bool GetIsBattleFinish()const
	{
		return m_isBattleFinish;
	}
	/// <summary>
	/// 戦闘終了フラグを設定。
	/// </summary>
	/// <param name="isBattleFinish"> 戦闘終了フラグ。</param>
	static void SetIsBattleFinish(bool isBattleFinish)
	{
		m_isBattleFinish = isBattleFinish;
	}



protected:
	Vector3 m_position = Vector3::Zero;				// 座標。
	Quaternion m_rotation = Quaternion::Identity;	// 回転。
	Vector3 m_scale = Vector3::One;					// 拡大率。
	ModelRender m_modelRender;						// モデルレンダラー。

	static bool m_isBattleFinish;					// 戦闘終了のフラグ。
	static bool m_isPause;							// ポーズのフラグ。


public:
	Actor() {};
	virtual ~Actor() {};


protected:
	virtual bool Start() override { return true; };
	virtual void Update() override {};
	virtual void Render(RenderContext& rc) override {};
};

