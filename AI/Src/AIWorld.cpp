#include "Precompiled.h"
#include "AIWorld.h"

using namespace AI;

void AI::AIWorld::Initialize()
{

}

void AI::AIWorld::Update()
{

}

void AI::AIWorld::Register(Entity* entity)
{
	XASSERT(std::find(mEntities.begin(), mEntities.end(), entity) == mEntities.end(), " AIWorld: entity at the end");
	mEntities.push_back(entity);
}

void AI::AIWorld::Unregister(Entity* entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end())
	{
		mEntities.erase(iter);
	}
}

void AI::AIWorld::AddObstacle(const X::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AI::AIWorld::AddWall(const X::Math::LineSegment& wall)
{
	mWalls.push_back(wall);
}

bool AI::AIWorld::HasLLineOfSight(const X::Math::LineSegment& lineSegment) const
{
	for (auto& wall : mWalls)
	{
		if (X::Math::Intersect(lineSegment, wall))
			return false;
	}

	for (auto& obstacle : mObstacles)
	{
		if (X::Math::Intersect(lineSegment, obstacle))
			return false;
	}

	return true;
}

EntityPtrs AI::AIWorld::GetEntitiesInRange(const X::Math::Circle& range, uint32_t typeId)
{
	float radiusSq = range.radius * range.radius;
	EntityPtrs entities;

	for (auto& entity : mEntities)
	{
		if (typeId == 0 || entity->GetTypeId() == typeId)
		{
			auto distVec = entity->position - range.center;
			if (X::Math::MagnitudeSqr(distVec) <= radiusSq)
			{
				entities.push_back(entity);
			}
		}
	}

	return entities;
}
