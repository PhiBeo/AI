#include "Precompiled.h"
#include "SeekBehavior.h"
#include "Agent.h"

X::Math::Vector2 AI::SeekBehavior::Calculate(Agent& agent)
{
	const auto agentToDest = agent.destination - agent.position;
	const float distToDest = X::Math::Magnitude(agentToDest);

	if (distToDest <= .1f)
	{
		return X::Math::Vector2::Zero();
	}

	const auto desiredVelocity = (agentToDest / distToDest) * agent.maxSpeed;
	const auto seekForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 1.0f, X::Colors::Green);

		return seekForce;
	}
}
