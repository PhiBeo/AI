#pragma once

#include <AI.h>

#include "VisualSensor.h"

enum class GatherMachineStates
{
	GoAndFindRawFood,
	GatheringRawFood,
	GoBackToFactory,
	ProcessRawFood,
	FleeFromChaser
};

class VisualSensor;

class GatherMachine : public AI::Agent
{
public:
	GatherMachine(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ShowDebug(bool debug);

	void ChangeState(GatherMachineStates pNewState);

	void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
	void SetWander(bool active) { mWanderBehavior->SetActive(active); }
	void SetFlee(bool active) { mFleeBehavior->SetActive(active); }

	void SetHome(X::Math::Vector2 pos) { homePosition = pos; }
	void SetFarm(X::Math::Vector2 pos) { farmPosition = pos; }
	void FinishFood() { mFoundFood = false; }

	X::Math::Vector2 GetFarmPos() { return farmPosition; }
	X::Math::Vector2 GetHomePos() { return homePosition; }
	bool HasFoundFood() { return mFoundFood; }
	bool HasReachFarm() { return position.x == farmPosition.x && position.y == farmPosition.y; }
	bool EnoughFood() { return currentFood >= foodCapacity; }
	bool IsHome() { return position.x == homePosition.x && position.y == homePosition.y; }
	bool IsNearChaser() {return X::Math::Distance(position, target->position) <= mFleeBehavior->GetPanicDistance()}

	float currentFood = 0;
private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;

	VisualSensor* mVisualSensor = nullptr;

	AI::StateMachine<GatherMachine>* mStateMachine;
	AI::WanderBehivior* mWanderBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::FleeBehavior* mFleeBehavior = nullptr;

	std::array<X::TextureId, 16> mTextureIds;

	float mWanderRadius = 20.f;
	float mWanderDistance = 50.f;
	float mWanderJitter = 5.f;

	float mViewRange;
	float mViewAngle;

	X::Math::Vector2 homePosition;
	X::Math::Vector2 farmPosition;

	bool mFoundFood = false;

	float foodCapacity = 50.f;
};