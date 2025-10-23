/// <summary>
/// 変形エネミーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class TransformEnemy;

namespace app
{
	namespace transformEnemy
	{
		class IdleState : public IState
		{
		public:
			IdleState(TransformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class WalkState : public IState
		{
		public:
			WalkState(TransformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class TransformEnemyStateMachine : public StateMachineBase
		{
		private:
			TransformEnemy* m_owner = nullptr;

		public:
			TransformEnemyStateMachine(TransformEnemy* owner) : m_owner(owner)
			{
				AddState<IdleState, TransformEnemy>(enEnemyState_Idle, owner);
				AddState<WalkState, TransformEnemy>(enEnemyState_Walk, owner);
			}

			void Update() override final;
		};
	}
}
