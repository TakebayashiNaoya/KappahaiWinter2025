/// <summary>
/// エネミーを管理するクラス。
/// </summary>
#pragma once
#include "Source/Actor/Character/Enemy/Enemy.h"

namespace app {
	namespace bossEnemy {
		class BossEnemyStateMachine;
	}
}

class BossEnemy : public Enemy
{
public:
	BossEnemy();
	~BossEnemy();

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// PlayerStateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Dead,	// ジャンプアニメーション。
		enAnimationClip_Num,
	};

	void DeleteEnemy() override final;

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;

	// ボスエネミーのステートマシン。
	std::unique_ptr<app::bossEnemy::BossEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption BOSS_ENEMY_ANIMATION_OPTIONS[];
};