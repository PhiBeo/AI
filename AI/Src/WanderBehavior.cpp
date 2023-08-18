#include "Precompiled.h"
#include "WanderBehavior.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::WanderBehivior::Calculate(Agent& agent)
{
	auto newWanderTarget = mLocalWanderTarget + (X::RandomUnitCircle() * mWanderJitter);

	newWanderTarget = X::Math::Normalize(newWanderTarget) * mWanderRadius;
	mLocalWanderTarget = newWanderTarget;

	newWanderTarget += X::Math::Vector2(0.f, mWanderDistance);

	const auto worldTransform = agent.GetWorldTransform();
	const auto worldWanderTarget = X::Math::TransformCoord(newWanderTarget, worldTransform);

	const auto agentDist = worldWanderTarget - agent.position;
	const float distToDest = X::Math::Magnitude(agentDist);
	if (distToDest <= 0.01f)
	{
		return X::Math::Vector2::Zero();
	}

	const auto desiredVelocity = (agentDist / distToDest) * agent.maxSpeed;
	const auto seekForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		const auto wanderCenter = X::Math::TransformCoord({ 0.0f, mWanderDistance }, worldTransform);
		X::DrawScreenCircle(wanderCenter, mWanderRadius, X::Colors::Yellow);
		X::DrawScreenDiamond(worldWanderTarget, 3.0f, X::Colors::Red);
		X::DrawScreenLine(agent.position, worldWanderTarget, X::Colors::Green);
	}

	return seekForce;
}

void AI::WanderBehivior::Setup(float radius, float distance, float jitter)
{
	mWanderDistance = distance;
	mWanderJitter = jitter;
	mWanderRadius = radius;
}
