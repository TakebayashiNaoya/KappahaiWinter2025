/// <summary>
/// プレイヤーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/Character.h"

namespace app {
	namespace player {
		class PlayerStateMachine;
	}
}

class Player :public Character
{
public:
	Player();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,		// 待機アニメーション。
		enAnimationClip_Walk,		// 歩きアニメーション。
		enAnimationClip_Run,		// 走りアニメーション。
		enAnimationClip_Jump,		// ジャンプアニメーション。
		enAnimationClip_KneelDown,	// 倒れるアニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// 踏みつけ用のコライダーのポインタを取得します。
	/// </summary>
	/// <returns> 踏みつけ用のコライダーのポインタ。</returns>
	CollisionObject* GetStompCollider()
	{
		return m_stompCollider;
	}

	/// <summary>
	/// プレイヤーの座標を取得します。
	/// </summary>
	/// <returns>プレイヤーの現在の座標値（float型）。</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// 毎フレームのXZ軸回転角度を取得します。
	/// </summary>
	/// <returns>XZ軸回転角度。</returns>
	const Quaternion& GetAdditionalRot() const
	{
		return m_xzAdditionalRot;
	}

	/// <summary>
	/// 攻撃されているかどうかを取得します。
	/// </summary>
	/// <returns> 攻撃されている場合はtrue、されていない場合はfalseを返す。</returns>
	const bool GetIsAttacked() const
	{
		return m_isAttacked;
	}

	/// <summary>
	/// 無敵中かどうかを取得します。
	/// </summary>
	/// <returns> 無敵中ならtrue、そうでなければfalseを返す。</returns>
	const bool GetIsInvincible() const
	{
		return m_isInvincible;
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
	/// 攻撃されているどうかのフラグを設定します。
	/// </summary>
	/// <param name="isAttacked"> 攻撃されている場合はtrue、されていない場合はfalseを入れる。</param>
	void SetIsAttacked(const bool isAttacked)
	{
		m_isAttacked = isAttacked;
	}

	/// <summary>
	/// 無敵中かどうかを設定します。
	/// </summary>
	/// <param name="isInvincible"> 無敵中ならtrue、そうでなければfalse。</param>
	void SetIsInvincible(const bool isInvincible)
	{
		m_isInvincible = isInvincible;
	}

	/// <summary>
	/// ノックバックタイマーをリセットします。
	/// </summary>
	void ResetKnockBackTimer()
	{
		m_knockedbackTimer = 0.0f;
	}

	/// <summary>
	/// カメラの回転角度を計算します。
	/// </summary>
	void CalcCameraRotation();

	/// <summary>
	/// 入力方向、速度、重力に基づいてキャラクターを移動させます。
	/// </summary>
	/// <param name="speed">移動速度を表す値。</param>
	void MoveUpdate(const float speed);

	/// <summary>
	/// 踏みつけ判定用コライダーを作成します。
	/// </summary>
	void CreateStompCollider();

	/// <summary>
	/// 踏みつけ判定用コライダーの座標と回転を更新します。
	/// </summary>
	void UpdateStompCollider();

	/// <summary>
	/// 踏みつけ判定用コライダーをdelete、nullptrします。
	/// </summary>
	void DeleteStompCollider();

	/// <summary>
	/// 攻撃してきた敵の方向を計算します。
	/// </summary>
	/// <param name="enemyPos"> 敵の座標。</param>
	void ComputeAttackedDirection(const Vector3& enemyPos);

	/// <summary>
	/// ノックバックされる処理を実行する関数。
	/// </summary>
	void KnockedBack();

	/// <summary>
	/// エネミーを踏みつけたときのジャンプ処理。
	/// </summary>
	void StompJump();

	/// <summary>
	/// ライフを1減らします。
	/// </summary>
	void TakeDamage();

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	CollisionObject* m_stompCollider = nullptr;		// 踏みつけ用のゴーストオブジェクト。

	Quaternion	m_xzAdditionalRot;					// 毎フレームのXZ軸回転角度（カメラの回転に使用）。

	bool m_isAttacked = false;						// ダメージを受けたかどうか。
	Vector3 m_attackedDirection = Vector3::Zero;	// ダメージを受けた方向。
	float	m_knockedbackTimer = 0.0f;				// ノックバックタイマー。
	bool m_isBlinking = false;						// 点滅中かどうか。
	bool m_isInvincible = false;					// 無敵状態かどうか。
	float   m_invincibleTimer = 0.0f;				// 無敵タイマー。

	/// プレイヤーのステートマシン。
	std::unique_ptr<app::player::PlayerStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption PLAYER_ANIMATION_OPTIONS[];

	/// <summary>
	/// 移動方向を返します。
	/// </summary>
	/// <returns>移動方向。</returns>
	const Vector3 ComputeMoveDirection()const;

	/// <summary>
	/// 移動方向に速度を乗算して返します。
	/// </summary>
	/// <param name="speed"> 移動速度。</param>
	/// <returns> 移動先の相対座標。</returns>
	const Vector3 CalcVelocity(const float speed)const;

	/// <summary>
	/// 一定時間が経過したら無敵状態を解除します。
	/// </summary>
	void InvincibleTimer();
};