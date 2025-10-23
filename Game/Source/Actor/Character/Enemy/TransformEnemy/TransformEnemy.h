/// <summary>
/// 変形エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Enemy/Enemy.h"
#include "Source/Actor/Character/Types.h"

namespace app {
	namespace transformEnemy {
		class TransformEnemyStateMachine;
	}
}

class TransformEnemy : public Enemy
{
public:
	TransformEnemy();

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
	/// 変形エネミーの座標を取得します。
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

	// 変形エネミーのステートマシン。
	std::unique_ptr<app::transformEnemy::TransformEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption TRANSFORM_ENEMY_ANIMATION_OPTIONS[];
};