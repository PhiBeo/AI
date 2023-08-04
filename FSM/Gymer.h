#pragma once
#include <AI.h>


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
	void DebugUI();
	void ChangeState(GymerStates pNewState);

	void SetLocation(LocationType location);

	bool IsEnoughHour();
	bool IsTired();
	bool IsSleeped();
	bool IsThirsty();

	void IncreaseHour();
	void IncreaseThirst();
	void ConsumeEnergy();
	void Awake();

	void ResetEnergy();
	void Sleeped();
	void ResetThirst();

	int GetEnergy() { return mEnergy; }
	int GetThirst() { return mthirst; }
	int GetWorkoutTime() { return mWorkoutTime; }

private:
	AI::StateMachine<Gymer>* mStateMachine;
	LocationType mLocation;

	int mthirst;
	bool msleeped;
	int mEnergy;
	int mWorkoutTime;

};
