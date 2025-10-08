/// <summary>
/// エネミーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class Enemy;

namespace app
{
	namespace enemy
	{
		app::IState* m_currentState = nullptr;	// 現在の状態を格納する変数。
		//app::IState* m_nextState = nullptr;		// 次の状態を格納する変数。

		/// <summary>
		/// プレイヤーの状態を管理するインターフェース。
		/// </summary>
		//app::IState* m_states[enPlayerState_Num];

		class IdleState : public IState
		{
		public:
			IdleState(Enemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class WalkState : public IState
		{
		public:
			WalkState(Enemy* owner) : IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class EnemyStateMachine : public StateMachineBase
		{
		private:
			Enemy* m_owner = nullptr;

		public:
			EnemyStateMachine(Enemy* owner) : m_owner(owner)
			{
				AddState<IdleState, Enemy>(enEnemyState_Idle, owner);
				AddState<WalkState, Enemy>(enEnemyState_Walk, owner);
			}

			void Update() override final;
		};
	}
}
