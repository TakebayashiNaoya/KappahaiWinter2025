/// <summary>
/// 基本エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Enemy/Enemy.h"

namespace app {
	namespace basicEnemy {
		class BasicEnemyStateMachine;
	}
}

class BasicEnemy : public Enemy
{
public:
	BasicEnemy();
	~BasicEnemy();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Walk,	// 歩きアニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// 基本エネミーの座標を取得します。
	/// </summary>
	/// <returns>プレイヤーの現在の座標値（float型）。</returns>
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// クールダウン状態かどうかを取得します。
	/// </summary>
	/// <returns> クールダウン状態であれば true。</returns>
	const bool GetIsOnCoolDown() const
	{
		return m_isCoolDown;
	}

	/// <summary>
	/// クールダウン状態を設定します。
	/// </summary>
	/// <param name="isCoolDown"> クールダウン状態の場合はtrueを設定。</param>
	void SetIsCoolDown(const bool isCoolDown)
	{
		m_isCoolDown = isCoolDown;
	}

	/// <summary>
	/// プレイヤーに向かって走ります。
	/// </summary>
	void ChasePlayer();

	/// <summary>
	/// クールダウンをカウントダウンし、一定時間が経過したらm_isCoolDownをfalseにします。
	/// </summary>
	void CoolDownCount();


private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;


	// 基本エネミーのステートマシン。
	std::unique_ptr<app::basicEnemy::BasicEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption BASIC_ENEMY_ANIMATION_OPTIONS[];

	float m_coolDownTimer = 0.0f;	// クールダウンタイマー。
	bool m_isCoolDown = false;		// クールダウン中かどうか。

	float m_deleteTimer = 0.0f;		// 死亡タイマー。
};