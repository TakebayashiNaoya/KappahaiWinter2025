#include "stdafx.h"
#include "BossEnemy.h"
#include "BossEnemyStateMachine.h"


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
			if (GetOwner<BossEnemy>()->GetIsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			if (GetOwner<BossEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enTransformEnemyState_Escape;
				return true;
			}
		}




		/*************************************/


		void AttackState::Enter()
		{
			GetOwner<BossEnemy>()->PlayAnimation(BossEnemy::enAnimationClip_Run);
		}


		void AttackState::Update()
		{
			GetOwner<BossEnemy>()->ChasePlayer();
			GetOwner<BossEnemy>()->ModelRotation();
		}


		void AttackState::Exit()
		{
		}


		bool AttackState::RequestState(int& requestStateId)
		{
			if (GetOwner<BossEnemy>()->GetIsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			// プレイヤーが一定距離外に出たら待機状態へ移行
			if (!GetOwner<BossEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enBossEnemyState_Idle;
				return true;
			}
			return false;
		}




		/*************************************/


		void DieState::Enter()
		{
			GetOwner<BossEnemy>()->DeleteEnemy();
		}


		void DieState::Update()
		{
		}


		void DieState::Exit()
		{
		}


		bool DieState::RequestState(int& requestStateId)
		{
			return true;
		}
	}
}