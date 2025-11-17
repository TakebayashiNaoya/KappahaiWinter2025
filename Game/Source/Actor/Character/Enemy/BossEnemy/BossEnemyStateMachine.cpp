#include "stdafx.h"
#include "BossEnemy.h"
#include "BossEnemyStateMachine.h"

namespace
{
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

			// クールダウンタイマーを更新。
			GetOwner<BossEnemy>()->UpdateCooldown();
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

			// クールダウン中は状態遷移しない。
			if (GetOwner<BossEnemy>()->IsOnCooldown()) {
				return false;
			}

			// プレイヤーが一定距離内に入ったら攻撃状態へ移行
			if (GetOwner<BossEnemy>()->GetDistanceToPlayer() < WALK_RANGE) {
				requestStateId = enBossEnemyState_Walk;
				return true;
			}
			// プレイヤーが一定距離内に入ったら走る状態へ移行
			else if (GetOwner<BossEnemy>()->GetDistanceToPlayer() < RUN_RANGE) {
				requestStateId = enBossEnemyState_Run;
				return true;
			}
			//else {
			//	//requestStateId = enBossEnemyState_Walk;
			//	return true;
			//}

			// クールダウンタイマーをリセット。
			GetOwner<BossEnemy>()->ResetCooldownTimer();
			return false;
		}




		/*************************************/


		void WalkState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Walk);
		}


		void WalkState::Update()
		{
			GetOwner<BossEnemy>()->ChasePlayer();
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void WalkState::Exit()
		{
		}


		bool WalkState::RequestState(int& requestStateId)
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


		void RunState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Run);
		}


		void RunState::Update()
		{
			GetOwner<BossEnemy>()->ChasePlayer();
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
				requestStateId = enBasicEnemyState_Die;
				return true;
			}

			if (GetOwner<BossEnemy>()->IsPlayingAnimation()) {
				return false;
			}

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
				requestStateId = enBasicEnemyState_Die;
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