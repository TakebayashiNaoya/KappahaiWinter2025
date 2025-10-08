#include "stdafx.h"
#include "StateMachine.h"


namespace app
{
	void StateMachineBase::Update()
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
}