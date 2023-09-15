#include "VisualSensor.h"

void VisualSensor::Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime)
{
	const float viewRangeSqr = viewRange * viewRange;
	const auto& entities = agent.world.GetEntities();

	for (auto& entity : entities)
	{
		if (entity == &agent)
			continue;

		X::Math::Vector2 posToTarget = entity->position - agent.position;
		if (X::Math::Magnitude(posToTarget) > viewRangeSqr)
		{
			continue;
		}

		X::Math::Vector2 dirToTarget = X::Math::Normalize(posToTarget);
		float dot = X::Math::Dot(agent.heading, dirToTarget);
		float angleToTarget = acos(dot);
		if (angleToTarget > viewHalfAngle)
			continue;



		auto iter = memory.begin();
		for (; iter != memory.end(); ++iter)
		{
			if (iter->uniqueId == entity->GetUniqueID())
			{
				break;
			}
		}

		if (iter != memory.end())
		{
			iter->properties["lastSeenPosition"] = entity->position;
			iter->lastRecordTime = X::GetTime();
		}
		else
		{
			auto& newRecord = memory.emplace_back();
			newRecord.uniqueId = entity->GetUniqueID();
			newRecord.properties["lastSeenPosition"] = entity->position;
			newRecord.lastRecordTime = X::GetTime();
		}	
	}

	auto fovStart = X::Math::Rotate(agent.heading * viewRange, -viewHalfAngle);
	auto fovEnd = X::Math::Rotate(agent.heading * viewRange, viewHalfAngle);

	X::DrawScreenLine(agent.position, agent.position + fovStart, X::Colors::Green);
	X::DrawScreenLine(agent.position, agent.position + fovEnd, X::Colors::Green);

	auto angle = atan2(agent.heading.y, agent.heading.x);

	X::DrawScreenArc(agent.position, viewRange, angle - viewHalfAngle, angle + viewHalfAngle, X::Colors::LightGreen);
}
