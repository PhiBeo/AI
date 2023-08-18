#pragma once

#include "SteeringBehavior.h"

namespace AI
{
	class SteeringModule final
	{
	public:
		SteeringModule(Agent& agent);

		template<class NewBehaviorType>
		NewBehaviorType* AddBehavior()
		{
			static_assert(std::is_base_of_v<SteeringBehavior, NewBehaviorType>, "SteeringBehavior: can only add steering behavior");
			auto& newBehavior = mBehaviors.emplace_back(std::make_unique<NewBehaviorType>());
			return static_cast<NewBehaviorType*>(newBehavior.get());
		}

		X::Math::Vector2 Calculate();


	private:
		Agent& mAgent;

		using SteeringBehaviors = std::vector<std::unique_ptr<SteeringBehavior>>;
		SteeringBehaviors mBehaviors;
	};
}