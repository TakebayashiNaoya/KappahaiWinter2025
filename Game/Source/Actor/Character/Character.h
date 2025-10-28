/// <summary>
/// キャラクターの基底クラス。
/// </summary>
#pragma once

class Character :public IGameObject
{
public:
	/// <summary>
	/// ボディのコライダーを取得します。
	/// </summary>
	/// <returns> ボディのコライダーのポインタ。</returns>
	CollisionObject* GetBodyCollider()
	{
		return m_bodyCollider;
	}

	/// <summary>
	/// 「惑星の中心→キャラ」の向きを取得します。
	/// </summary>
	/// <returns>「惑星の中心→キャラ」の向き。</returns>
	const Vector3& GetUpDirection() const
	{
		return m_upDirection;
	}

	/// <summary>
	/// キャラクターの座標を取得します。
	/// </summary>
	/// <returns>キャラクターの座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// キャラクターの回転を取得します。
	/// </summary>
	/// <returns>キャラクターの回転。</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/// <summary>
	/// 接地しているかを取得します。
	/// 地面に向かってレイを飛ばし、当たった座標と自分の座標の距離が一定未満なら接地していると判定します。
	/// </summary>
	/// <returns>接地していれば true、そうでなければ false を示す。</returns>
	const bool& IsOnGround();

	/// <summary>
	/// ジャンプ前の速度を取得します。
	/// </summary>
	/// <returns>ジャンプ前の速度を表す float 型の定数参照。</returns>
	const float& GetSpeedBeforeJump() const
	{
		return m_speedBeforeJump;
	}

	/// <summary>
	/// ジャンプ前の移動速度を設定します。
	/// </summary>
	void SetSpeedBeforeJump(const float speed)
	{
		m_speedBeforeJump = speed;
	}

	/// <summary>
	/// 落下タイマーをリセットします。
	/// </summary>
	void ResetFallTimer()
	{
		m_fallTimer = 0.0f;
	}

	/// <summary>
	/// 指定されたアニメーション番号のアニメーションを再生します。
	/// </summary>
	/// <param name="animNo">再生するアニメーションの番号。</param>
	void PlayAnimation(const int animNo);

	/// <summary>
	/// ジャンプパワーをジャンプスピードとムーブスピードに即座に適用します。
	/// </summary>
	/// <param name="jumpPower">ジャンプパワー。</param>
	void ApplyJumpImpulse(const float jumpPower);

	/// <summary>
	/// キャラクターを指定された速度で移動させます。
	/// </summary>
	/// <param name="speed">移動速度。</param>
	void MoveOnGround(const float speed);

	/// <summary>
	/// 重力処理と移動処理の両方を行います。
	/// 空中での移動速度はジャンプ前の移動速度を維持します。
	/// </summary>
	void MoveOffGround();

	/// <summary>
	/// moveSpeedに基づいてモデルを回転させます。
	/// </summary>
	void ModelRotation();

	/// <summary>
	/// ゴーストオブジェクトを更新します。
	/// </summary>
	/// <param name="offset">位置補正の値。</param>
	void UpdateBodyCollider(const float offset);

	/// <summary>
	/// ボディコライダーをdelete、nullptrします。
	/// </summary>
	void DeleteBodyCollider();

protected:
	AnimationClip* m_animationClips = nullptr;						// アニメーションクリップ。
	CollisionObject* m_bodyCollider = nullptr;						// キャラクター同士の当たり判定用ゴーストオブジェクト。


	ModelRender	m_modelRender;										// モデルレンダー。
	Vector3		m_position = Vector3::Zero;							// ポジション。
	Quaternion  m_rotation = Quaternion::Identity;					// 回転。
	int			m_life = 0;											// ライフ。

	//---ジャンプ・重力関連---//
	float		m_speedBeforeJump = 0.0f;							// ジャンプ前の移動速度。
	float		m_initialJumpSpeed = 0.0f;							// ジャンプ速度。
	float		m_fallTimer = 0.0f;									// 落下時間。
	Vector3		m_lastHitPosition = Vector3::Zero;					// 最後に地面に接地した位置。

	//---移動・回転関連---//
	Vector3     m_moveSpeed = Vector3::Zero;						// 移動速度。
	Vector3     m_planetCenter = Vector3::Zero;						// 重力を働かせる惑星の中心座標。
	Vector3     m_upDirection = Vector3::Zero;						// 惑星の中心から自分への方向ベクトル。
	Vector3     m_beforeDirectionFromPlanetCenter = Vector3::Zero;	// 前フレームの惑星の中心から自分への方向ベクトル。


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
	/// <param name="path">モデルファイルのパス。</param>
	void InitModel(const size_t count, const AnimationOption* option, const std::string path);

	/// <summary>
	/// ライフを設定する。
	/// </summary>
	/// <param name="life">設定するライフの値（整数）。</param>
	void InitLife(const int life)
	{
		m_life = life;
	}

	/// <summary>
	/// 「惑星の中心→キャラ」のベクトルを計算し、正規化します。
	/// </summary>
	void UpdateUpDirection();

	// ベクトル v を法線 n の接平面へ投影（接線成分を取り出す）
	// Dot(v, n) は v と n の内積 → v の中で n 方向にどれだけ成分があるか。
	// n * Dot(v, n) はその成分を n 方向に戻したベクトル。
	// v - (その成分) → n方向の成分を引いて、残りを返す → 結果は n に直交する平面上のベクトル（接線）
	static Vector3 ProjectOnPlane(const Vector3& v, const Vector3& n)
	{
		return v - n * Dot(v, n);
	}
};

