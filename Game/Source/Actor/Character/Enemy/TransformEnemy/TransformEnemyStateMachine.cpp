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
		}


		void IdleState::Update()
		{
		}


		void IdleState::Exit()
		{
		}


		bool IdleState::RequestState(int& requestStateId)
		{
			return false;
		}




		/*************************************/


		void WalkState::Enter()
		{
		}


		void app::transformEnemy::WalkState::Update()
		{
		}


		void app::transformEnemy::WalkState::Exit()
		{
		}


		bool app::transformEnemy::WalkState::RequestState(int& requestStateId)
		{
			return false;
		}
	}
}