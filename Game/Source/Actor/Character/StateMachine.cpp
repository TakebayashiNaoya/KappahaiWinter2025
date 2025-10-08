#include "stdafx.h"
#include "StateMachine.h"


namespace app
{
	void StateMachineBase::Update()
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
}