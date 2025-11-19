/// <summary>
/// 変形エネミーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class DeformEnemy;

namespace app
{
	namespace deformEnemy
	{
		class IdleState : public IState
		{
		public:
			IdleState(DeformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class EscapeState : public IState
		{
		public:
			EscapeState(DeformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class TransformState : public IState
		{
		public:
			TransformState(DeformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class SlideState : public IState
		{
		public:
			SlideState(DeformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class DieState : public IState
		{
		public:
			DieState(DeformEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class DeformEnemyStateMachine : public StateMachineBase
		{
		private:
			DeformEnemy* m_owner = nullptr;

		public:
			DeformEnemyStateMachine(DeformEnemy* owner) : m_owner(owner)
			{
				AddState<IdleState, DeformEnemy>(enDeformEnemyState_Idle, owner);
				AddState<EscapeState, DeformEnemy>(enDeformEnemyState_Escape, owner);
				AddState<TransformState, DeformEnemy>(enDeformEnemyState_Transform, owner);
				AddState<SlideState, DeformEnemy>(enDeformEnemyState_Slide, owner);
				AddState<DieState, DeformEnemy>(enDeformEnemyState_Die, owner);
			}

			void Update() override final;
		};
	}
}
