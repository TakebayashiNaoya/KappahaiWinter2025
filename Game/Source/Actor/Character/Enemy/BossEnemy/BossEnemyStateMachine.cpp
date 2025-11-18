#include "stdafx.h"
#include "BossEnemy.h"
#include "BossEnemyStateMachine.h"

namespace
{
	constexpr float WALK_SPEED = 8.0f;		// 歩く速度
	constexpr float RUN_SPEED = 17.0f;		// 走る速度

	constexpr float WALK_RANGE = 500.0f;	// 歩く範囲。
	constexpr float RUN_RANGE = 1000.0f;	// 歩く範囲。
	constexpr float ATTACK_RANGE = 200.0f;	// 攻撃範囲。
}

namespace app
{
	namespace bossEnemy
	{
		void BossEnemyStateMachine::Update()
		{
			StateMachineBase::Update();
		}




		/*************************************/


		void IdleState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Idle);
		}


		void IdleState::Update()
		{
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			// 死亡していたら死亡状態へ移行。
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBossEnemyState_Dead;
				return true;
			}

			// プレイヤーが一定距離内に入ったら攻撃状態へ移行
			if (GetOwner<BossEnemy>()->GetDistanceToPlayer() < RUN_RANGE) {
				requestStateId = enBossEnemyState_Walk;
				return true;
			}

			// プレイヤーが一定距離内に入ったら走る状態へ移行
			else if (GetOwner<BossEnemy>()->GetDistanceToPlayer() > RUN_RANGE) {
				requestStateId = enBossEnemyState_Run;
				return true;
			}

			return false;
		}




		/*************************************/


		void CooldownState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Idle);

			// クールダウンタイマーをリセット。
			GetOwner<BossEnemy>()->ResetCooldownTimer();
		}


		void CooldownState::Update()
		{
			GetOwner<BossEnemy>()->ModelRotation();

			// クールダウンタイマーを更新。
			GetOwner<BossEnemy>()->UpdateCooldown();
		}


		void CooldownState::Exit()
		{
			// クールダウンタイマーをリセット。
			GetOwner<BossEnemy>()->ResetCooldownTimer();
		}


		bool CooldownState::RequestState(int& requestStateId)
		{
			// 死亡していたら死亡状態へ移行。
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBossEnemyState_Dead;
				return true;
			}

			// クールダウン中は状態遷移しない。
			if (GetOwner<BossEnemy>()->IsOnCooldown()) {
				return false;
			}

			// クールダウンが終了したら待機状態へ移行。
			requestStateId = enBossEnemyState_Idle;
			return true;
		}




		/*************************************/


		void WalkState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Walk);
		}


		void WalkState::Update()
		{
			GetOwner<BossEnemy>()->ChasePlayer(WALK_SPEED);
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void WalkState::Exit()
		{
		}


		bool WalkState::RequestState(int& requestStateId)
		{
			// 死亡していたら死亡状態へ移行。
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}

			// プレイヤーが一定距離内に入ったら攻撃状態へ移行
			if (GetOwner<BossEnemy>()->GetDistanceToPlayer() < ATTACK_RANGE) {
				requestStateId = enBossEnemyState_Attack;
				return true;
			}

			// プレイヤーが一定距離離れたら走る状態へ移行
			else if (GetOwner<BossEnemy>()->GetDistanceToPlayer() > RUN_RANGE) {
				requestStateId = enBossEnemyState_Run;
				return true;
			}

			return false;
		}




		/*************************************/


		void RunState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Run);
		}


		void RunState::Update()
		{
			GetOwner<BossEnemy>()->ChasePlayer(RUN_SPEED);
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void RunState::Exit()
		{
		}


		bool RunState::RequestState(int& requestStateId)
		{
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}

			if (GetOwner<BossEnemy>()->GetDistanceToPlayer() < ATTACK_RANGE) {
				requestStateId = enBossEnemyState_Attack;
				return true;
			}

			return false;
		}




		/*************************************/


		void AttackState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Attack);
		}


		void AttackState::Update()
		{
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void AttackState::Exit()
		{
		}


		bool AttackState::RequestState(int& requestStateId)
		{
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBossEnemyState_Dead;
				return true;
			}

			if (GetOwner<BossEnemy>()->IsPlayingAnimation()) {
				return false;
			}

			requestStateId = enBossEnemyState_Cooldown;
			return true;

			return false;
		}




		/*************************************/


		void DamageState::Enter()
		{
			// ダメージアニメーション再生
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Damage);
		}


		void DamageState::Update()
		{
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void DamageState::Exit()
		{
		}


		bool DamageState::RequestState(int& requestStateId)
		{
			if (GetOwner<BossEnemy>()->IsDead()) {
				requestStateId = enBossEnemyState_Dead;
				return true;
			}
			// ダメージアニメーションが終了したら待機状態へ移行
			// ここでは仮に常にアニメーションが終了したとみなす
			requestStateId = enBossEnemyState_Idle;
			return true;
		}




		/*************************************/


		void DeadState::Enter()
		{
			DeleteGO(GetOwner<BossEnemy>());
		}


		void DeadState::Update()
		{
		}


		void DeadState::Exit()
		{
		}


		bool DeadState::RequestState(int& requestStateId)
		{
			return true;
		}





	}
}