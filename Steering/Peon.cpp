#include "Peon.h"
#include "TypeIds.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
}

void Peon::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehivior>();
	mArriveBehavior = mSteeringModule->AddBehavior<AI::ArriveBehavior>();
	mSeperationBehavior = mSteeringModule->AddBehavior<AI::SeperationBehavior>();
	mAlignmentBehavior = mSteeringModule->AddBehavior<AI::AlignmentBehavior>();
	mCohesionBehavior = mSteeringModule->AddBehavior<AI::CohesionBehavior>();
	mWanderBehavior->SetActive(true);
	mSeperationBehavior->SetActive(true);
	mAlignmentBehavior->SetActive(true);
	mCohesionBehavior->SetActive(true);

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}

	float spriteWidth = X::GetSpriteWidth(mTextureIds[0]);
	radius = (spriteWidth * 0.5f) + 5.0f;
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

	if (position.x < 0.0f)
	{
		position.x += screenWidth;
	}
	if (position.x >= screenWidth)
	{
		position.x -= screenWidth;
	}
	if (position.y < .0f)
	{
		position.y += screenHeight;
	}
	if (position.y >= screenHeight)
	{
		position.y -= screenHeight;
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
	mWanderBehavior->ShowDebug(debug);
	mSeekBehavior->ShowDebug(debug);
	mFleeBehavior->ShowDebug(debug);
	mArriveBehavior->ShowDebug(debug);
	mSeperationBehavior->ShowDebug(debug);
	mAlignmentBehavior->ShowDebug(debug);
	mCohesionBehavior->ShowDebug(debug);
}
