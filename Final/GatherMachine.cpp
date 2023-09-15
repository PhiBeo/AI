#include "GatherMachine.h"
#include "TypeIds.h"
#include "GatherMachineStates.h"
#include "VisualSensor.h"
#include "MemoryRecord.h"

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		return record.importance;
	}
}

GatherMachine::GatherMachine(AI::AIWorld& world)
	: Agent(world, Types::GatherId)
{
}

void GatherMachine::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = Types::FoodId;
	mPerceptionModule->SetMemorySpan(2.f);

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehavior = mSteeringModule->AddBehavior<AI::SeekBehavior>();
	mWanderBehavior = mSteeringModule->AddBehavior<AI::WanderBehivior>();
	mFleeBehavior = mSteeringModule->AddBehavior<AI::FleeBehavior>();
	mWanderBehavior->SetActive(true);

	mStateMachine = new AI::StateMachine<GatherMachine>(*this);
	
	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}

	float spriteWidth = X::GetSpriteWidth(mTextureIds[0]);
	radius = (spriteWidth * 0.5f) + 5.0f;

	mStateMachine->AddState<GoAndFindRawFoodState>();
	mStateMachine->AddState<GatheringRawFoodState>();
	mStateMachine->AddState<GoBackToFactoryState>();
	mStateMachine->AddState<ProcessRawFoodState>();
	mStateMachine->AddState<FleeFromChaserState>();
	mStateMachine->ChangeState(0);

	position = X::RandomVector2({ 100.f, 100.f }, { X::GetScreenWidth() - 100.f, X::GetScreenHeight() - 100.f });
	maxSpeed = 200.0f;
}

void GatherMachine::Unload()
{
	delete mStateMachine;
	mStateMachine = nullptr;
}

void GatherMachine::Update(float deltaTime)
{
	mVisualSensor->viewRange = mViewRange;
	mVisualSensor->viewHalfAngle = mViewAngle * X::Math::kDegToRad;

	mStateMachine->Update(deltaTime);
	mPerceptionModule->Update(deltaTime);

	if (mWanderBehavior->IsActive())
	{
		mWanderBehavior->Setup(mWanderRadius, mWanderDistance, mWanderJitter);
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
		farmPosition = pos;
		mFoundFood = true;
	}
}

void GatherMachine::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();

	X::Math::Circle pos;
	pos.center = position;
	pos.radius = radius;

	X::DrawScreenCircle(pos, X::Colors::AntiqueWhite);

	X::DrawSprite(mTextureIds[frame], position);
}

void GatherMachine::ShowDebug(bool debug)
{
	mWanderBehavior->ShowDebug(debug);
	mFleeBehavior->ShowDebug(debug);
	mSeekBehavior->ShowDebug(debug);
}

void GatherMachine::ChangeState(GatherMachineStates pNewState)
{
	mStateMachine->ChangeState((int)pNewState);
}

bool GatherMachine::IsReach(Types type)
{
	X::Math::Circle selfBox;
	selfBox.center = position;
	selfBox.radius = radius;

	auto entities = world.GetEntitiesInRange(selfBox, type);

	if (!entities.empty()) return true;

	return false;
}
