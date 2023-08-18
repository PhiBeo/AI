#include "Precompiled.h"
#include "Entity.h"

#include "AIWorld.h"

using namespace AI;

AI::Entity::Entity(AIWorld& world, uint32_t typeId) :
	world(world),
	mUniqueId(static_cast<uint64_t>(typeId) << 32 | world.GetNextId())
{
	world.Register(this);
}

AI::Entity::~Entity()
{
	world.Unregister(this);
}

X::Math::Matrix3 AI::Entity::GetWorldTransform() const
{
	const auto h = heading;
	const auto s = X::Math::PerpendicularRH(h);
	const auto d = position;

	return{
		s.x, s.y,0.0,
		h.x,h.y,0.0,
		d.x, d.y, 1.0f
	};
}
