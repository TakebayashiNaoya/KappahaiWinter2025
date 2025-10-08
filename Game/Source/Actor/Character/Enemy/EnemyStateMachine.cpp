#include "stdafx.h"
#include "Source/Actor/Character/Enemy/Enemy.h"
#include "Source/Actor/Character/Enemy/EnemyStateMachine.h"


namespace app
{
	namespace enemy
	{
		void EnemyStateMachine::Update()
		{
			int requestStateId = 0;
			// ��ԕύX�̃��N�G�X�g�������True���A���Ă���
			if (m_currentState->RequestState(requestStateId))
			{
				// ���݂̏�Ԃ��I������
				m_currentState->Exit();
				// ���̏�Ԃ֐؂�ւ�
				m_currentState = FindState<IState>(requestStateId);
				// ���̏�Ԃ��J�n����
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
			// �ǂ�������Ⴄ�������ŏ���
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


		void app::enemy::WalkState::Update()
		{
		}


		void app::enemy::WalkState::Exit()
		{
		}


		bool app::enemy::WalkState::RequestState(int& requestStateId)
		{
			return false;
		}
	}
}