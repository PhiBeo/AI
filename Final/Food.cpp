#include "Food.h"
#include "TypeIds.h"

Food::Food(AI::AIWorld& world)
	:Agent(world, Types::FoodId)
{
}

void Food::Load()
{
	mTextureId = X::LoadTexture("carrot.png");
	position = X::RandomVector2({ 100.f, 100.f }, { X::GetScreenWidth() - 100.f, X::GetScreenHeight() - 100.f });
	radius = 20.0f;
	wasGathered = false;
}

void Food::Update(float deltaTime)
{

	if (!wasGathered && IsInteract(Types::GatherId))
	{
		wasGathered = true;
	}
	else if (wasGathered && !IsInteract(Types::GatherId))
	{
		wasGathered = false;
		position = X::RandomVector2({ 100.f, 100.f }, { X::GetScreenWidth() - 100.f, X::GetScreenHeight() - 100.f });
	}
}

void Food::Render()
{
	X::DrawSprite(mTextureId, position);
}

bool Food::IsInteract(Types type)
{
	X::Math::Circle selfBox;
	selfBox.center = position;
	selfBox.radius = radius;

	auto entities = world.GetEntitiesInRange(selfBox, type);

	if (!entities.empty()) return true;

	return false;
}
