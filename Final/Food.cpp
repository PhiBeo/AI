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
}

void Food::Render()
{
	X::DrawSprite(mTextureId, position);
}
