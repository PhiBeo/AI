#include "Gymer.h"
#include "GymerStates.h"

void Gymer::Initialize()
{
	mLocation = LocationType::Home;
	mthirst = 0;
	msleeped = false;
	mEnergy = 0;
	mWorkoutTime = 0;

	mStateMachine = new AI::StateMachine<Gymer>(*this);

	mStateMachine->AddState<GoToGymState>();
	mStateMachine->AddState<EnterTheCaffetteriaState>();
	mStateMachine->AddState<GoHomeAndSleepState>();
	mStateMachine->AddState<GetSomeWaterState>();
	mStateMachine->ChangeState(0);
}

void Gymer::Terminate()
{
	delete mStateMachine;
	mStateMachine = nullptr;
}

void Gymer::Update(float deltaTime)
{
	mStateMachine->Update(deltaTime);
}

void Gymer::DebugUI()
{
	mStateMachine->DebugUI();
}

void Gymer::ChangeState(GymerStates pNewState)
{
	mStateMachine->ChangeState((int)pNewState);
}

void Gymer::SetLocation(LocationType location)
{
	mLocation = location;
}

bool Gymer::IsEnoughHour()
{
	return mWorkoutTime > 20;
}

bool Gymer::IsTired()
{
	return mEnergy >= 8;
}

bool Gymer::IsSleeped()
{
	return msleeped;
}

bool Gymer::IsThirsty()
{
	return mthirst >= 4;
}

void Gymer::IncreaseHour()
{
	mWorkoutTime++;
}

void Gymer::IncreaseThirst()
{
	mthirst++;
}

void Gymer::ConsumeEnergy()
{
	mEnergy++;
}

void Gymer::Awake()
{
	msleeped = false;
}

void Gymer::ResetEnergy()
{
	mEnergy = 0;
}

void Gymer::Sleeped()
{
	msleeped = true;
	mWorkoutTime = 0;
	mthirst = 0;
	mEnergy = 0;
}

void Gymer::ResetThirst()
{
	mthirst = 0;
}
