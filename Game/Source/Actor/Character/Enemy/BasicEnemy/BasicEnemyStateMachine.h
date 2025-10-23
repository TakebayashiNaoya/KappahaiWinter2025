/// <summary>
/// 基本エネミーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class BasicEnemy;

namespace app
{
	namespace basicEnemy
	{
		class IdleState : public IState
		{
		public:
			IdleState(BasicEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class WalkState : public IState
		{
		public:
			WalkState(BasicEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class BasicEnemyStateMachine : public StateMachineBase
		{
		private:
			BasicEnemy* m_owner = nullptr;

		public:
			BasicEnemyStateMachine(BasicEnemy* owner) : m_owner(owner)
			{
				AddState<IdleState, BasicEnemy>(enEnemyState_Idle, owner);
				AddState<WalkState, BasicEnemy>(enEnemyState_Walk, owner);
			}

			void Update() override final;
		};
	}
}
