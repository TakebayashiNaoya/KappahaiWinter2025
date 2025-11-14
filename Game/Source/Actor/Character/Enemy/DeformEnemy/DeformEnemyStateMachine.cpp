#include "stdafx.h"
#include "DeformEnemy.h"
#include "DeformEnemyStateMachine.h"


namespace app
{
	namespace deformEnemy
	{
		void DeformEnemyStateMachine::Update()
		{
			StateMachineBase::Update();
		}




		/*************************************/


		void IdleState::Enter()
		{
			GetOwner<DeformEnemy>()->PlayAnimation(DeformEnemy::enAnimationClip_Idle);
		}


		void IdleState::Update()
		{
			GetOwner<DeformEnemy>()->ModelRotation();
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			if (GetOwner<DeformEnemy>()->IsDead()) {
				requestStateId = enDeformEnemyState_Die;
				return true;
			}
			if (GetOwner<DeformEnemy>()->IsFoundPlayer()) {
				requestStateId = enDeformEnemyState_Escape;
				return true;
			}
		}




		/*************************************/


		void EscapeState::Enter()
		{
			GetOwner<DeformEnemy>()->PlayAnimation(DeformEnemy::enAnimationClip_Run);
		}


		void EscapeState::Update()
		{
			GetOwner<DeformEnemy>()->EscapePlayer();
			GetOwner<DeformEnemy>()->ModelRotation();
		}


		void EscapeState::Exit()
		{
		}


		bool EscapeState::RequestState(int& requestStateId)
		{
			if (GetOwner<DeformEnemy>()->IsDead()) {
				requestStateId = enDeformEnemyState_Die;
				return true;
			}
			if (GetOwner<DeformEnemy>()->IsDeformed()) {
				requestStateId = enDeformEnemyState_Transform;
				return true;
			}
			if (!GetOwner<DeformEnemy>()->IsFoundPlayer()) {
				requestStateId = enDeformEnemyState_Idle;
				return true;
			}
			return false;
		}




		/*************************************/


		void TransformState::Enter()
		{
			GetOwner<DeformEnemy>()->PlayAnimation(DeformEnemy::enAnimationClip_Dead);
		}


		void TransformState::Update()
		{
		}


		void TransformState::Exit()
		{
		}


		bool TransformState::RequestState(int& requestStateId)
		{
			if (GetOwner<DeformEnemy>()->IsDead()) {
				requestStateId = enDeformEnemyState_Die;
				return true;
			}
			if (GetOwner<DeformEnemy>()->IsSliding()) {
				requestStateId = enDeformEnemyState_Slide;
				return true;
			}
			return false;
		}




		/*************************************/


		void SlideState::Enter()
		{
		}


		void SlideState::Update()
		{
			GetOwner<DeformEnemy>()->Sliding();
			GetOwner<DeformEnemy>()->ModelRotation();
		}


		void SlideState::Exit()
		{
		}


		bool SlideState::RequestState(int& requestStateId)
		{
			if (GetOwner<DeformEnemy>()->IsDead()) {
				requestStateId = enDeformEnemyState_Die;
				return true;
			}
			if (!GetOwner<DeformEnemy>()->IsSliding()) {
				requestStateId = enDeformEnemyState_Transform;
				return true;
			}
			return false;
		}




		/*************************************/


		void DieState::Enter()
		{
			GetOwner<DeformEnemy>()->DeleteEnemy();
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