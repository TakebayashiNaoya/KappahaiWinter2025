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

		class EscapeState : public IState
		{
		public:
			EscapeState(TransformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class TransformState : public IState
		{
		public:
			TransformState(TransformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class SlideState : public IState
		{
		public:
			SlideState(TransformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class DieState : public IState
		{
		public:
			DieState(TransformEnemy* owner) : IState(owner) {}

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
				AddState<IdleState, TransformEnemy>(enTransformEnemyState_Idle, owner);
				AddState<EscapeState, TransformEnemy>(enTransformEnemyState_Escape, owner);
				AddState<TransformState, TransformEnemy>(enTransformEnemyState_Transform, owner);
				AddState<SlideState, TransformEnemy>(enTransformEnemyState_Slide, owner);
				AddState<DieState, TransformEnemy>(enTransformEnemyState_Die, owner);
			}

			void Update() override final;
		};
	}
}
