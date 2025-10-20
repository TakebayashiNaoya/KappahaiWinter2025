#include "stdafx.h"
#include "BasicEnemy.h"
#include "BasicEnemyStateMachine.h"


namespace app
{
	namespace basicEnemy
	{
		void BasicEnemyStateMachine::Update()
		{
			int requestStateId = 0;
			// 状態変更のリクエストがあればTrueが帰ってくる
			if (m_currentState->RequestState(requestStateId))
			{
				// 現在の状態を終了する
				m_currentState->Exit();
				// 次の状態へ切り替え
				m_currentState = FindState<IState>(requestStateId);
				// 次の状態を開始する
				m_currentState->Enter();
			}
			m_currentState->Update();
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
			// どこかから貰う情報を仮で書く
			bool canRun = true;
			if (canRun) {
				requestStateId = enPlayerState_Run;
				return true;
			}

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