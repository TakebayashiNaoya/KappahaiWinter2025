/// <summary>
/// キャラクターの基底クラス。
/// </summary>
#pragma once

class Character :public IGameObject
{
public:
	/// <summary>
	/// キャラクターコントローラーへの参照を取得します。
	/// </summary>
	/// <returns>キャラクターコントローラー（CharacterController）への定数参照。</returns>
	const CharacterController& GetCharacterController() const
	{
		return m_characterController;
	}

	/// <summary>
	/// 指定されたアニメーション番号のアニメーションを再生します。
	/// </summary>
	/// <param name="animNo">再生するアニメーションの番号。</param>
	void PlayAnimation(const int animNo);

	/// <summary>
	///　ジャンプパワーをジャンプスピードとムーブスピードに即座に適用します。
	/// </summary>
	/// <param name="jumpPower">ジャンプパワー。</param>
	void ApplyJumpImpulse(const float jumpPower);

	/// <summary>
	/// キャラクターを指定された速度で移動させます。
	/// </summary>
	/// <param name="speed">移動速度。</param>
	void Move(float speed);

	/// <summary>
	/// moveSpeedに基づいてY軸回転を更新します。
	/// </summary>
	void Rotation();

protected:
	ModelRender m_modelRender;							// モデルレンダー。
	Vector3 m_position = Vector3::Zero;					// ポジション。
	Quaternion m_rotation;								// 回転。
	Vector3 m_moveSpeed = Vector3::Zero;				// 移動速度。
	AnimationClip* m_animationClips;					// アニメーションクリップ。
	CharacterController m_characterController;			// キャラクターコントローラー。
	float m_jumpSpeed = 0.0f;							// ジャンプの速さ。
	Vector3 m_planetCenter = Vector3::Zero;				// 重力を働かせる惑星の中心座標。
	Vector3 m_directionToPlanetCenter = Vector3::Zero;	// 自分から惑星の中心への方向ベクトル。
	Vector3 m_directionFromPlanetCenter = Vector3::Zero;// 惑星の中心から自分への方向ベクトル。

	// 静的メンバとして宣言
	// 継承先の.cppで定義すること。
	static const std::string ANIMATION_FILE_PATH;
	static const std::string ANIMATION_EXTENSION;

	/// <summary>
	/// アニメーションファイルのオプションを管理する構造体。
	/// 継承先の.cppで定義すること。
	/// </summary>
	struct AnimationOption
	{
		const char* fileName;	// ファイルパス。
		bool is_loop = false;	// リピートするかどうか。（true=する、false=しない）

		/// <summary>
		/// アニメーションファイルのフルパスを取得します。
		/// </summary>
		std::string GetFullPath() const
		{
			return ANIMATION_FILE_PATH + fileName + ANIMATION_EXTENSION;
		}
	};

	/// <summary>
	/// キャラクターのモデルとアニメーションクリップを初期化します。
	/// </summary>
	/// <param name="count">アニメーションクリップの数。</param>
	/// <param name="option">各アニメーションクリップの設定情報が格納されたAnimationOption型の配列。</param>
	void InitModel(const size_t count, const AnimationOption* option);

	/// <summary>
	/// 重力を適用します。
	/// </summary>
	void AddGravity();

	/// <summary>
	/// 「キャラ→惑星の中心」のベクトルを計算し、正規化します。
	/// </summary>
	void CalcDirectionToPlanetCenter();

	/// <summary>
	/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	/// </summary>
	void CalcDirectionFromPlanetCenter();

	// ベクトル v を法線 n の接平面へ投影（接線成分を取り出す）
	// Dot(v, n) は v と n の内積 → v の中で n 方向にどれだけ成分があるか。
	// n * Dot(v, n) はその成分を n 方向に戻したベクトル。
	// v - (その成分) → n方向の成分を引いて、残りを返す → 結果は n に直交する平面上のベクトル（接線）
	static Vector3 ProjectOnPlane(const Vector3& v, const Vector3& n)
	{
		return v - n * Dot(v, n);
	}
};

