#pragma once
#include "State.h"

namespace AI
{
	template<class AgentType>
	class StateMachine final
	{
	public:
		StateMachine(AgentType& agent) :
			mAgent(agent)
		{
			
		}
		template<class NewState>
		void AddState()
		{
			static_assert(std::is_base_of_v<StateType, NewState>, "StateMachine: can only add one state");
			mStates.push_back(std::make_unique<NewStateType>());
		}
		void Update(float deltaTime)
		{
			if (mCurrentState != nullptr)
			{
				mCurrentState->Update(mAgent, deltaTime);
			}
		}
		void ChangeState(int index)
		{
			if (mCurrentState != nullptr)
			{
				mCurrentState->Exit(mAgent);
			}
			mCurrentState = mStates[index].get();
			mCurrentState->Enter(mAgent);
		}

	private:
		AgentType& mAgent;
		StateType* mCurrentState = nullptr;
		std::vector<std::unique_ptr<StateType>> mStates;
	};
}