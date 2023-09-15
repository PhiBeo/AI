#include "Peon.h"
#include "TypeIds.h"

#include "VisualSensor.h"
#include "MemoryRecord.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

extern float viewRange;
extern float viewAngle;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		float distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
		float distanceScore = 100000.0f - distance;
		return distanceScore;
	}
}

Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
}

void Peon::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehivior>();
	mWanderBehavior->SetActive(true);

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
	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;

	mPerceptionModule->Update(deltaTime);

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

	const auto& memmoryRecords = mPerceptionModule->GetMemoryRecords();

	for (auto& memory : memmoryRecords)
	{
		auto pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
		X::DrawScreenLine(position, pos, X::Colors::Aquamarine);
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
}
