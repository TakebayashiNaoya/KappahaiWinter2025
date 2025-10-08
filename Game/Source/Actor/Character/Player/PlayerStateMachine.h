/// <summary>
/// プレイヤーのステートマシン。
/// </summary>
#pragma once
#include "Source/Actor/Character/Types.h"
#include "Source/Actor/Character/StateMachine.h"

class Player;

namespace app
{
	namespace player
	{
		class IdleState : public app::IState
		{
		public:
			// :IState(owner) これはコンストラクタ。
			IdleState(Player* owner) : app::IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class WalkState : public app::IState
		{
		public:
			WalkState(Player* owner) : app::IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class RunState : public app::IState
		{
		public:
			RunState(Player* owner) : app::IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class JumpState : public app::IState
		{
		public:
			JumpState(Player* owner) : app::IState(owner) {}

			void Enter() override final;
			void Update() override final;
			void Exit() override final;
			bool RequestState(int& requestStateId) override final;
		};

		class PlayerStateMachine : public StateMachineBase
		{
		private:
			Player* m_owner = nullptr;


		public:
			PlayerStateMachine(Player* owner) : m_owner(owner)
			{
				AddState<IdleState, Player>(enPlayerState_Idle, owner);
				AddState<WalkState, Player>(enPlayerState_Walk, owner);
				AddState<RunState, Player>(enPlayerState_Run, owner);
				AddState<JumpState, Player>(enPlayerState_Jump, owner);
			}

			void Update() override final;
		};
	}
}
