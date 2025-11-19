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

	/// <summary>
	/// アニメーションのクリップを表す列挙型です。
	/// StateMachineでアニメを切り替えるために使用します。
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,	// 待機アニメーション。
		enAnimationClip_Walk,	// 待機アニメーション。
		enAnimationClip_Run,	// 走りアニメーション。
		enAnimationClip_Attack,	// 攻撃アニメーション。
		enAnimationClip_Damage,	// ダメージアニメーション。
		enAnimationClip_Dead,	// 死亡アニメーション。
		enAnimationClip_Num,
	};

	/// <summary>
	/// プレイヤーに向かって走ります。
	/// </summary>
	/// <param name="speed">移動速度。</param>
	void ChasePlayer(const float speed);

	/// <summary>
	/// クールダウンを更新します。
	/// </summary>
	void UpdateCooldown();
	/// <summary>
	/// 現在クールダウン中かどうかを判定します。
	/// </summary>
	/// <returns>クールダウン中であればtrue、そうでなければfalseを返します。</returns>
	const bool IsOnCooldown()const;
	/// <summary>
	/// クールダウンタイマーをリセットします。
	/// </summary>
	const void ResetCooldownTimer();

	/// <summary>
	/// プレイヤーまでの距離を計算して返します。const メンバー関数であり、オブジェクトの状態は変更されません。
	/// </summary>
	/// <returns>プレイヤーまでの距離を表す float 値を返します（戻り値は const）。</returns>
	const float GetDistanceToPlayer()const;

	/// <summary>
	/// アニメーションイベントを処理します。
	/// </summary>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	bool Start() override final;
	void Update() override final;
	void Render(RenderContext& rc) override final;


	Vector3 m_lastPlayerPos = Vector3::Zero;	// 最後にプレイヤーを見た位置。
	float m_cooldownTimer = 0.0f;				// クールダウンタイマー。

	//CollisionObject* m_attackCollider = nullptr;	// 攻撃コライダー。

	// ボスエネミーのステートマシン。
	std::unique_ptr<app::bossEnemy::BossEnemyStateMachine> m_stateMachine;

	// クラススコープで宣言し、cppで定義。
	static const Character::AnimationOption BOSS_ENEMY_ANIMATION_OPTIONS[];
};