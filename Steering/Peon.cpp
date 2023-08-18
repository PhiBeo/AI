#include "Peon.h"
#include "TypeIds.h"

float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
}

void Peon::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	//mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehivior>();
	mWanderBehavior->SetActive(true);

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Peon::Unload()
{

}

void Peon::Update(float deltaTime)
{
	if (mWanderBehavior->IsActive())
	{
		mWanderBehavior->Setup(wanderRadius, wanderDistance, wanderJitter);
	}

	const auto force = mSteeringModule->Calculate();
	const auto acceleration = force / mass;
	velocity += acceleration * deltaTime;

	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	position += velocity * deltaTime;

	const auto screenWidth = X::GetScreenWidth();
	const auto screenHeight = X::GetScreenHeight();

	if (position.x < .0f)
	{
		position.x = screenWidth;
	}
	if (position.x > screenWidth)
	{
		position.x = screenWidth;
	}
	if (position.y < .0f)
	{
		position.y = screenHeight;
	}
	if (position.y < screenHeight)
	{
		position.y = screenHeight;
	}

}

void Peon::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();

	X::DrawSprite(mTextureIds[frame], position);
}

void Peon::ShowDebug(bool debug)
{
	mWanderBehavior->IsDebug();
}
