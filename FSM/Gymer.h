#pragma once
#include <AI.h>

#include "GymerStates.h"

enum GymerStates
{
	GoToGym,
	EnterTheCaffetteria,
	GoHomeAndSleep,
	GetSomeWater
};

class Gymer
{
public:
	enum LocationType
	{
		Gym,
		Caffetteria,
		Home,
		WaterDispenser
	};

	Gymer() = default;

	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void ChangeState(GymerStates pNewState);

	void SetLocation(LocationType location);

	bool IsEnoughHour();
	bool IsTired();
	bool IsSleeped();
	bool IsThirsty();

	void IncreaseHour();
	void IncreaseThirst();
	void DecreaseEnergy(int amount);
	void Awake();

	void ResetEnergy();
	void Sleeped();
	void ResetThirst();

private:
	AI::StateMachine<Gymer>* mStateMachine;
	LocationType mLocation;

	int mthirst;
	bool msleeped;
	int mEnergy;
	int mWorkoutTime;

};
