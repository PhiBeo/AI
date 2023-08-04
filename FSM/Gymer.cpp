#include "Gymer.h"

void Gymer::Initialize()
{
	mLocation = LocationType::Home;
	mthirst = 0;
	msleeped = false;
	mEnergy = 180;
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
	return mWorkoutTime > 180;
}

bool Gymer::IsTired()
{
	return mEnergy <= 0;
}

bool Gymer::IsSleeped()
{
	return msleeped;
}

bool Gymer::IsThirsty()
{
	return mthirst > 100;
}

void Gymer::IncreaseHour()
{
	mWorkoutTime++;
}

void Gymer::IncreaseThirst()
{
	mthirst += 1;
}

void Gymer::DecreaseEnergy(int amount)
{
	mEnergy -= amount;
}

void Gymer::Awake()
{
	msleeped = false;
}

void Gymer::ResetEnergy()
{
	mEnergy = 180;
}

void Gymer::Sleeped()
{
	msleeped = true;
}

void Gymer::ResetThirst()
{
	mthirst = 0;
}
