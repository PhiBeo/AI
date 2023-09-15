#pragma once
#include <AI.h>

#include "VisualSensor.h"

enum class ChaseMachineStates
{
	WanderAround,
	GoHome,
	Sleep,
	ChaseGatherer
};

class VisualSensor;

class ChaseMachine : public AI::Agent
{
public:
	ChaseMachine(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ShowDebug(bool debug);

	void ChangeState(ChaseMachineStates pNewState);

	void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
	void SetWander(bool active) { mWanderBehavior->SetActive(active); }
	void SetHome(X::Math::Vector2 pos) { homePos = pos; }

	X::Math::Vector2 GetHome() { return homePos; }
	bool IsDetect() { return detectGatherer; }
	bool IsReach(Types type);
	X::Math::Vector2 GetDetectedPos() { return detectPos; }


	AI::StateMachine<ChaseMachine>* GetStateMachine() { return mStateMachine; }

	float wanderingTime = 0.0f;
	float sleepTime = 0.0f;
private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;

	VisualSensor* mVisualSensor = nullptr;

	AI::StateMachine<ChaseMachine>* mStateMachine;
	AI::WanderBehivior* mWanderBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;

	std::array<X::TextureId, 32> mTextureIds;

	float mWanderRadius = 20.f;
	float mWanderDistance = 50.f;
	float mWanderJitter = 5.f;

	float mViewRange = 200.0f;
	float mViewAngle = 40.f;

	X::Math::Vector2 homePos;

	bool detectGatherer = false;
	X::Math::Vector2 detectPos = X::Math::Vector2::Zero();
	
};