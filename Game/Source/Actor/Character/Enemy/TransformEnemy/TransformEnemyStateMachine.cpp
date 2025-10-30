#include "stdafx.h"
#include "TransformEnemy.h"
#include "TransformEnemyStateMachine.h"


namespace app
{
	namespace transformEnemy
	{
		void TransformEnemyStateMachine::Update()
		{
			StateMachineBase::Update();
		}




		/*************************************/


		void IdleState::Enter()
		{
			GetOwner<TransformEnemy>()->PlayAnimation(TransformEnemy::enAnimationClip_Idle);
		}


		void IdleState::Update()
		{
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			if (GetOwner<TransformEnemy>()->GetIsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			if (GetOwner<TransformEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enTransformEnemyState_Escape;
				return true;
			}
		}




		/*************************************/


		void EscapeState::Enter()
		{
			GetOwner<TransformEnemy>()->PlayAnimation(TransformEnemy::enAnimationClip_Run);
		}


		void EscapeState::Update()
		{
			GetOwner<TransformEnemy>()->EscapePlayer();
			GetOwner<TransformEnemy>()->ModelRotation();
		}


		void EscapeState::Exit()
		{
		}


		bool EscapeState::RequestState(int& requestStateId)
		{
			if (GetOwner<TransformEnemy>()->GetIsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			if (GetOwner<TransformEnemy>()->GetIsTransform()) {
				requestStateId = enTransformEnemyState_Transform;
				return true;
			}
			if (!GetOwner<TransformEnemy>()->GetIsFoundPlayer()) {
				requestStateId = enTransformEnemyState_Idle;
				return true;
			}
			return false;
		}




		/*************************************/


		void TransformState::Enter()
		{
			GetOwner<TransformEnemy>()->PlayAnimation(TransformEnemy::enAnimationClip_Jump);
		}


		void TransformState::Update()
		{
		}


		void TransformState::Exit()
		{
		}


		bool TransformState::RequestState(int& requestStateId)
		{
			if (GetOwner<TransformEnemy>()->GetIsDead()) {
				requestStateId = enBasicEnemyState_Die;
				return true;
			}
			if (GetOwner<TransformEnemy>()->GetIsSliding()) {
				requestStateId = enTransformEnemyState_Slide;
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
			GetOwner<TransformEnemy>()->Sliding();
			GetOwner<TransformEnemy>()->ModelRotation();
		}


		void SlideState::Exit()
		{
		}


		bool SlideState::RequestState(int& requestStateId)
		{
			if (GetOwner<TransformEnemy>()->GetIsDead()) {
				requestStateId = enTransformEnemyState_Die;
				return true;
			}
			return false;
		}




		/*************************************/


		void DieState::Enter()
		{
			GetOwner<TransformEnemy>()->DeleteEnemy();
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