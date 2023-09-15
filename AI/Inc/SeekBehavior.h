#pragma once

#include "SteeringBehavior.h"

namespace AI
{
	class SeekBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

	private:

	};

	class FleeBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
		float GetPanicDistance() { return panicDistance; }

	private:
		float panicDistance = 250.0f;
	};
}