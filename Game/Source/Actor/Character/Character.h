/// <summary>
/// キャラクターの基底クラス。
/// </summary>
#pragma once

enum EnCollisionType;

class Character :public IGameObject
{
public:
	/// <summary>
	/// 一時攻撃用のコライダーのポインタを取得します。
	/// </summary>
	/// <returns> コライダーのポインタ。</returns>
	CollisionObject* GetAttackCollider()const
	{
		return m_attackCollider;
	}
	/// <summary>
	/// 一時攻撃用のコライダーのポインタを設定します。
	/// </summary>
	/// <param name="collider"> コライダーのポインタ。</param>
	void SetAttackCollider(CollisionObject* collider)
	{
		m_attackCollider = collider;
	}
	/// <summary>
	/// 攻撃用のコライダーのポインタを設定します。
	/// </summary>
	/// <param name="collider"> コライダーのポインタ。</param>
	CollisionObject* GetHitCollider()const
	{
		return m_hitCollider;
	}
	/// <summary>
	/// やられ判定のコライダーを取得します。
	/// </summary>
	/// <returns> やられ判定のコライダーのポインタ。</returns>
	CollisionObject* GetHurtCollider()const
	{
		return m_hurtCollider;
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
	/// 攻撃されているかどうかを取得します。
	/// </summary>
	/// <returns> 攻撃されている場合はtrue、されていない場合はfalseを返す。</returns>
	const bool IsAttacked() const
	{
		return m_isAttacked;
	}
	/// <summary>
	/// 攻撃されているどうかのフラグを設定します。
	/// </summary>
	/// <param name="isAttacked"> 攻撃されている場合はtrue、されていない場合はfalseを入れる。</param>
	void SetIsAttacked(const bool isAttacked)
	{
		m_isAttacked = isAttacked;
	}


	/// <summary>
	/// 死亡状態を取得します。
	/// </summary>
	/// <returns> 死亡している場合はtrue、そうでない場合はfalseを返します。</returns>
	const bool IsDead() const
	{
		return m_isDead;
	}
	/// <summary>
	/// 死亡状態を設定します。
	/// </summary>
	/// <param name="isDead"> true の場合は死亡、false の場合は生存を表します。</param>
	void SetIsDead(const bool isDead)
	{
		m_isDead = isDead;
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
	/// アニメーションが再生中かどうかを返します。
	/// </summary>
	/// <returns>アニメーションが再生中であれば true、そうでなければ false を返します。</returns>
	const bool IsPlayingAnimation() const
	{
		return m_modelRender.IsPlayingAnimation();
	};

	/// <summary>
	/// ジャンプパワーをジャンプスピードとムーブスピードに即座に適用します。
	/// </summary>
	/// <param name="jumpPower">ジャンプパワー。</param>
	void ApplyJumpImpulse(const float jumpPower);


	/// <summary>
	/// moveSpeedに基づいてモデルを回転させます。
	/// </summary>
	void ModelRotation();


	/// <summary>
	/// ライフを1減らします。
	/// </summary>
	void TakeDamage()
	{
		m_life--;
	}

	/// <summary>
	/// ライフを取得します。
	/// </summary>
	/// <returns> ライフ。</returns>
	const int GetLife() const
	{
		return m_life;
	}


	/// <summary>
	/// 現在の座標に合わせて、強制的にキャラクターを惑星に対して直立させます。
	/// </summary>
	void ResetRotation();


protected:
	AnimationClip* m_animationClips = nullptr;						// アニメーションクリップ。
	ModelRender	m_modelRender;										// モデルレンダー。

	CollisionObject* m_hitCollider = nullptr;						// 攻撃判定。
	CollisionObject* m_hurtCollider = nullptr;						// やられ判定。
	CollisionObject* m_attackCollider = nullptr;					// 一時的な攻撃判定。

	Vector3		m_position = Vector3::Zero;							// ポジション。
	Quaternion  m_rotation = Quaternion::Identity;					// 回転。
	int			m_life = 0;											// ライフ。
	bool		m_isAttacked = false;								// ダメージを受けたかどうか。

	//---ジャンプ・重力関連---//
	float		m_speedBeforeJump = 0.0f;							// ジャンプ前の移動速度。
	float		m_initialJumpSpeed = 0.0f;							// ジャンプ速度。
	float		m_fallTimer = 0.0f;									// 落下時間。
	Vector3		m_lastHitPosition = Vector3::Zero;					// 最後に地面に接地した位置。

	//---移動・回転関連---//
	Vector3     m_moveSpeed = Vector3::Zero;						// 移動速度。
	Vector3     m_planetCenter = Vector3::Zero;						// 重力を働かせる惑星の中心座標。
	Vector3     m_upDirection = Vector3::Zero;						// 惑星の中心から自分への方向ベクトル。
	Vector3     m_beforeUpDirection = Vector3::Zero;				// 前フレームの惑星の中心から自分への方向ベクトル。


protected:
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
	/// <param name="scale">モデルの拡大率。（規定値は1.0f）</param>
	void InitModel(const size_t count, const AnimationOption* option, const std::string path, const float scale = 1.0f);


protected:
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


	/// <summary>
	/// ベクトル v を法線 n の接平面へ投影（接線成分を取り出す）
	/// Dot(v, n) は v と n の内積 → v の中で n 方向にどれだけ成分があるか。
	/// n * Dot(v, n) はその成分を n 方向に戻したベクトル。
	/// v - (その成分) → n方向の成分を引いて、残りを返す → 結果は n に直交する平面上のベクトル（接線）
	/// </summary>
	/// <param name="v"> 投影するベクトル。</param>
	/// <param name="n"> 法線ベクトル。</param>
	/// <returns> 接平面への投影ベクトル。</returns>
	static Vector3 ProjectOnPlane(const Vector3& v, const Vector3& n)
	{
		return v - n * Dot(v, n);
	}


	/// <summary>
	/// 移動方向を計算して返します。
	/// キャラによって移動方向の決定方法が異なるため、継承先でオーバーライドしてください。
	/// </summary>
	/// <returns> 移動方向。</returns>
	virtual const Vector3 ComputeMoveDirection() const
	{
		return Vector3::Zero;
	};


	/// <summary>
	/// 移動方向に速度を乗算して返します。
	/// ComputeMoveDirection()を使い、継承先で移動方向を決定してください。
	/// </summary>
	/// <param name="direction"> 移動方向。</param>
	/// <param name="speed"> 速度。</param>
	/// <returns> 水平方向の速度ベクトル。</returns>
	const Vector3 CalcHorizontalVelocity(const float speed)const
	{
		Vector3 velocity = ComputeMoveDirection() * speed;
		return velocity;
	};


	/// <summary>
	/// ジャンプや重力から、垂直方向の速度ベクトルを計算して返します。
	/// </summary>
	/// <returns> 垂直方向の速度。/returns>
	const Vector3 CalcVerticalVelocity();


	/// <summary>
	/// 移動速度から移動後の座標を計算します。
	/// </summary>
	void ComputePosition();
};