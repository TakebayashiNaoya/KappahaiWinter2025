/// <summary>
/// 変形エネミーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class BossEnemy;

namespace app
{
	namespace bossEnemy
	{
		class IdleState : public IState
		{
		public:
			IdleState(BossEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class AttackState : public IState
		{
		public:
			AttackState(BossEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class DieState : public IState
		{
		public:
			DieState(BossEnemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class BossEnemyStateMachine : public StateMachineBase
		{
		private:
			BossEnemy* m_owner = nullptr;

		public:
			BossEnemyStateMachine(BossEnemy* owner) : m_owner(owner)
			{
				AddState<IdleState, BossEnemy>(enBossEnemyState_Idle, owner);
				AddState<AttackState, BossEnemy>(enBossEnemyState_Attack, owner);
				AddState<DieState, BossEnemy>(enBossEnemyState_Die, owner);
			}

			void Update() override final;
		};
	}
}
