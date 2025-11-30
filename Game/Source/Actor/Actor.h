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


public:
	// ... (既存のGet/Set関数) ...

	/// <summary>
	/// ループ音の再生を開始します。
	/// </summary>
	/// <param name="soundId">再生するサウンドのID (EnSoundList)</param>
	void StartLoopSound(int soundId);

	/// <summary>
	/// ループ音を停止・破棄します。
	/// </summary>
	void StopLoopSound();

	/// <summary>
	/// ループ音の座標をActorの現在座標に更新します。
	/// Update内で呼び出してください。
	/// </summary>
	void UpdateLoopSound();


protected:
	Vector3 m_position = Vector3::Zero;				// 座標。
	Quaternion m_rotation = Quaternion::Identity;	// 回転。
	Vector3 m_scale = Vector3::One;					// 拡大率。
	ModelRender m_modelRender;						// モデルレンダラー。
	SoundSource* m_loopSound = nullptr;				// サウンドソース。


public:
	Actor() {};
	virtual ~Actor() {};


protected:
	virtual bool Start() override { return true; };
	virtual void Update() override {};
	virtual void Render(RenderContext& rc) override {};
};

