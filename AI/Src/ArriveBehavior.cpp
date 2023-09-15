#include "Precompiled.h"
#include "ArriveBehavior.h"
#include "Agent.h"

X::Math::Vector2 AI::ArriveBehavior::Calculate(Agent& agent)
{
	const auto agentToDest = agent.destination - agent.position;
	const float distToDest = X::Math::Magnitude(agentToDest);
	float speed = 0.0f;

	if (distToDest > 0.1f)
	{
		const float decelTweaker = 0.3f;
		float speed = distToDest / (((float)mDeceleration + 1.0f) * decelTweaker);
		speed = X::Math::Min(speed, agent.maxSpeed);
	}

	const auto desiredVelocity = (agentToDest / distToDest) * speed;
	const auto arriveForce = desiredVelocity - agent.velocity;


	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Green);
	}

	return arriveForce;
}

void AI::ArriveBehavior::SetDeceleration(Deceleration decel)
{
	mDeceleration = decel;
}
