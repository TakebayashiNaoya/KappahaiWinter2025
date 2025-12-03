/// <summary>
/// 変形エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Enemy/Enemy.h"

namespace app {
	namespace deformEnemy {
		class DeformEnemyStateMachine;
	}
}

class DeformEnemy : public Enemy
{
public:
	DeformEnemy();
	~DeformEnemy();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Walk,	// 歩きアニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Dead,	// 死亡アニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// 変形エネミーの座標を取得します。
	/// </summary>
	/// <returns>プレイヤーの現在の座標値（float型）。</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// 変形しているかどうかを取得します。
	/// </summary>
	/// <returns> 変形しているならtrueを返す。</returns>
	const bool IsDeformed() const
	{
		return m_isDeformed;
	}

	/// <summary>
	/// 滑走しているかどうかを取得します。
	/// </summary>
	/// <returns> 滑走しているならtrueを返す。</returns>
	const bool& IsSliding() const
	{
		return m_isSliding;
	}

	/// <summary>
	/// 変形しているかどうかを設定します。
	/// </summary>
	/// <param name="isTransform"> 変形しているならtrueを入れる。</param>
	void SetIsDeformed(const bool isDeformed)
	{
		m_isDeformed = isDeformed;
	}

	/// <summary>
	/// 滑走しているかどうかを設定します。
	/// </summary>
	/// <param name="isSliding"> 滑走しているならtrueを入れる。</param>
	void SetIsSliding(const bool isSliding)
	{
		m_isSliding = isSliding;
	}

	/// <summary>
	/// 衝撃を受けた位置から初期の滑走方向を計算します。
	/// </summary>
	/// <param name="impulsePos"> 衝撃を受けた座標。または衝撃を与えたものの座標。</param>
	void CalcInitialSlideDirection(const Vector3& impulsePos)
	{
		Vector3 direction = m_position - impulsePos;
		direction.Normalize();
		m_slideDirection = direction;
	}

	/// <summary>
	/// プレイヤーから逃げます。
	/// </summary>
	void EscapePlayer();

	/// <summary>
	/// 滑走します。
	/// </summary>
	void Sliding();


private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	/// <summary>
	/// 滑走する方向を更新します。
	/// </summary>
	void UpdateSlideDirection();

	// 変形エネミーのステートマシン。
	std::unique_ptr<app::deformEnemy::DeformEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption TRANSFORM_ENEMY_ANIMATION_OPTIONS[];

	bool m_isDeformed = false;					// 変形しているかどうか。
	bool m_isSliding = false;					// 滑っているかどうか。
	Vector3 m_slideDirection = Vector3::Zero;	// 滑走方向。
	Vector3 m_beforePosition = Vector3::Zero;	// 1フレーム前の座標。
};