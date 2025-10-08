/// <summary>
/// �X�e�[�g�}�V���̊��N���X�B
/// </summary>
#pragma once
#include <unordered_map>

namespace app
{
	class IState
	{
	protected:
		IGameObject* m_owner = nullptr;

	public:
		IState(IGameObject* owner) : m_owner(owner) {}

		// virtual�ŉ��z�֐��A�����=0�������珃�����z�֐��ɂȂ�B���̊֐����`���Y���ƃG���[���o��B
		virtual void Enter() = 0;
		virtual void Update() = 0;
		virtual void Exit() = 0;
		virtual bool RequestState(int& requestStateId) = 0;

		template <typename T>
		T* GetOwner()
		{
			return static_cast<T*>(m_owner);
		}
	};

	class StateMachineBase
	{
	protected:
		std::unordered_map<int, IState*> m_stateMap;
		app::IState* m_currentState = nullptr;	// ���݂̏�Ԃ��i�[����ϐ��B


	protected:
		virtual void Update();

	public:
		template<typename TState, typename TOwner>
		void AddState(const int stateId, TOwner* owner)
		{
			// TODO: �}�b�v�ւ̒ǉ����@�𒲂ׂ�
			// ������Ƃ�����
			// m_stateMap[stateId] = new T();
			m_stateMap.emplace(stateId, new TState(owner));
		}

		template <typename T>
		T* FindState(const int stateId)
		{
			// ������Ȃ������ꍇ��end()(null)������B
			auto it = m_stateMap.find(stateId);
			if (it != m_stateMap.end()) {
				// std::unordered_map<int, IState*> m_stateMap;
				// first��key(int)�Asecond��value(IState*)
				// IState*��T*�ɕϊ����ĕԂ�
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}

		// �ŏ��̃X�e�[�g��ݒ肷��
		void InitializeState(const int stateId)
		{
			m_currentState = FindState<IState>(stateId);
			if (m_currentState != nullptr) {
				m_currentState->Enter();
			}
		}
	};
}
