#include "Precompiled.h"
#include "GroupBehavior.h"
#include "Agent.h"

X::Math::Vector2 AI::SeperationBehavior::Calculate(Agent& agent)
{
	X::Math::Vector2 seperationForce;
	const float forceMultiplier = 5.0f;
	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			auto dirToNeighbor = n->position - agent.position;
			auto distance = X::Math::Magnitude(dirToNeighbor);
			auto overlapDistance = (n->radius + agent.radius) - distance;
			if (overlapDistance > 0.0f)
			{
				dirToNeighbor /= distance;
				if (X::Math::Dot(dirToNeighbor, agent.heading) > 0.0f)
				{
					float desiredSpeed = X::Math::Min((overlapDistance / agent.radius) * agent.maxSpeed * forceMultiplier, agent.maxSpeed);
					seperationForce += (-dirToNeighbor * desiredSpeed);
				}
			}
		}
	}

	if (IsDebug())
	{
		X::DrawScreenCircle(agent.position, agent.radius, X::Colors::SpringGreen);
	}

	return seperationForce;
}

X::Math::Vector2 AI::AlignmentBehavior::Calculate(Agent& agent)
{
	X::Math::Vector2 alignmentForce;
	X::Math::Vector2 averageHeading;
	int totalAgents = 0;
	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			if (X::Math::Dot(agent.heading, n->heading) > 0.0f)
			{
				averageHeading += n->heading;
				++totalAgents;
			}
		}
	}

	if (totalAgents > 0)
	{
		averageHeading /= static_cast<float>(totalAgents);
		alignmentForce = (averageHeading - agent.heading) * agent.maxSpeed;
	}

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + alignmentForce, X::Colors::Purple);
	}

	return alignmentForce;
}

X::Math::Vector2 AI::CohesionBehavior::Calculate(Agent& agent)
{
	X::Math::Vector2 cohesionForce;
	X::Math::Vector2 centerOfMass;
	int totalAgents = 0;
	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			centerOfMass += n->heading;
			++totalAgents;
		}
	}

	if (totalAgents > 0)
	{
		centerOfMass /= static_cast<float>(totalAgents);
		const auto diseredVelocity = X::Math::Normalize(centerOfMass) * agent.maxSpeed;
		cohesionForce = diseredVelocity - agent.velocity;
	}

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + cohesionForce, X::Colors::HotPink);
	}

	return cohesionForce;
}
