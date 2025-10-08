/// <summary>
/// ステートマシンの基底クラス。
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

		// virtualで仮想関数、さらに=0をつけたら純粋仮想関数になる。この関数を定義し忘れるとエラーが出る。
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
		app::IState* m_currentState = nullptr;	// 現在の状態を格納する変数。


	protected:
		virtual void Update();

	public:
		template<typename TState, typename TOwner>
		void AddState(const int stateId, TOwner* owner)
		{
			// TODO: マップへの追加方法を調べる
			// ちょっとちがう
			// m_stateMap[stateId] = new T();
			m_stateMap.emplace(stateId, new TState(owner));
		}

		template <typename T>
		T* FindState(const int stateId)
		{
			// 見つからなかった場合はend()(null)が入る。
			auto it = m_stateMap.find(stateId);
			if (it != m_stateMap.end()) {
				// std::unordered_map<int, IState*> m_stateMap;
				// firstがkey(int)、secondがvalue(IState*)
				// IState*をT*に変換して返す
				return static_cast<T*>(it->second);
			}
			return nullptr;
		}

		// 最初のステートを設定する
		void InitializeState(const int stateId)
		{
			m_currentState = FindState<IState>(stateId);
			if (m_currentState != nullptr) {
				m_currentState->Enter();
			}
		}
	};
}
