/// <summary>
/// 基本エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Enemy/Enemy.h"
#include "Source/Actor/Character/Types.h"

namespace app {
	namespace basicEnemy {
		class BasicEnemyStateMachine;
	}
}

class BasicEnemy : public Enemy
{
public:
	BasicEnemy();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Walk,	// 歩きアニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Jump,	// ジャンプアニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// 基本エネミーの座標を取得します。
	/// </summary>
	/// <returns>プレイヤーの現在の座標値（float型）。</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

private:
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;

	// 基本エネミーのステートマシン。
	std::unique_ptr<app::basicEnemy::BasicEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption BASIC_ENEMY_ANIMATION_OPTIONS[];
};