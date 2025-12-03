#include "stdafx.h"
#include "BasicEnemy.h"
#include "BasicEnemyStateMachine.h"


namespace app
{
	namespace basicEnemy
	{
		void BasicEnemyStateMachine::Update()
		{
			StateMachineBase::Update();
		}




		/*************************************/


		void IdleState::Enter()
		{
			GetOwner<BasicEnemy>()->PlayAnimation(BasicEnemy::enAnimationClip_Idle);
		}


		void IdleState::Update()
		{
			GetOwner<BasicEnemy>()->ModelRotation();
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			if (GetOwner<BasicEnemy>()->IsDying()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			if (GetOwner<BasicEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enBasicEnemyState_Chase;
				return true;
			}
			if (GetOwner<BasicEnemy>()->GetIsOnCoolDown()) {
				requestStateId = enBasicEnemyState_CoolDown;
				return true;
			}
			return false;
		}





		/*************************************/


		void ChaseState::Enter()
		{
			GetOwner<BasicEnemy>()->PlayAnimation(BasicEnemy::enAnimationClip_Run);
		}


		void app::basicEnemy::ChaseState::Update()
		{
			GetOwner<BasicEnemy>()->ChasePlayer();
			GetOwner<BasicEnemy>()->ModelRotation();
		}


		void app::basicEnemy::ChaseState::Exit()
		{
		}


		bool app::basicEnemy::ChaseState::RequestState(int& requestStateId)
		{
			if (GetOwner<BasicEnemy>()->IsDying()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			// プレイヤーが一定距離外に出たら待機状態へ移行
			if (!GetOwner<BasicEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enBasicEnemyState_Idle;
				return true;
			}
			if (GetOwner<BasicEnemy>()->GetIsOnCoolDown()) {
				requestStateId = enBasicEnemyState_CoolDown;
				return true;
			}
			return false;
		}




		/*************************************/


		void CoolDownState::Enter()
		{
			GetOwner<BasicEnemy>()->PlayAnimation(BasicEnemy::enAnimationClip_Idle);
		}


		void app::basicEnemy::CoolDownState::Update()
		{
			GetOwner<BasicEnemy>()->CoolDownCount();
		}


		void CoolDownState::Exit()
		{
		}


		bool CoolDownState::RequestState(int& requestStateId)
		{
			if (GetOwner<BasicEnemy>()->IsDying()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			// クールダウンが終わったら歩き状態へ移行
			if (!GetOwner<BasicEnemy>()->GetIsOnCoolDown()) {
				requestStateId = enBasicEnemyState_Idle;
				return true;
			}
			return false;
		}





		/*************************************/


		void DieState::Enter()
		{
			DeleteGO(GetOwner<BasicEnemy>());
		}


		void DieState::Update()
		{
		}


		void DieState::Exit()
		{
		}


		bool DieState::RequestState(int& requestStateId)
		{
			return false;
		}
	}
}