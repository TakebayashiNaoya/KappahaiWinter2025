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


		void app::basicEnemy::WalkState::Update()
		{
		}


		void app::basicEnemy::WalkState::Exit()
		{
		}


		bool app::basicEnemy::WalkState::RequestState(int& requestStateId)
		{
			return false;
		}
	}
}