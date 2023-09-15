#include "Home.h"
#include "TypeIds.h"

Home::Home(AI::AIWorld& world)
	:Agent(world, Types::HomeId)
{
}

void Home::Load()
{
	mTextureId = X::LoadTexture("refinery_05.png");
	position = X::RandomVector2({ 100.f, 100.f }, { X::GetScreenWidth() - 100.f, X::GetScreenHeight() - 100.f });
	radius = 20.0f;
}

void Home::Render()
{
	X::DrawSprite(mTextureId, position);
}
