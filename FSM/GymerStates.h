#pragma once
#include "AI.h"

#include "Gymer.h"

class GoToGymState : public AI::State<Gymer>
{
public:
	void Enter(Gymer& agent) override
	{
		agent.SetLocation(Gymer::LocationType::Gym);
	}

	void Update(Gymer& agent, float deltaTime) override
	{
		agent.DecreaseEnergy(1);
		agent.IncreaseHour();
		if (agent.IsTired())
		{
			agent.ChangeState(GymerStates::EnterTheCaffetteria);
		}
		else if (agent.IsThirsty())
		{
			agent.ChangeState(GymerStates::GetSomeWater);
		}
	}

	void Exit(Gymer& agent) override
	{
		//empty
	}
};

class EnterTheCaffetteriaState : public AI::State<Gymer>
{
public:
	void Enter(Gymer& agent) override
	{
		agent.SetLocation(Gymer::LocationType::Caffetteria);
	}

	void Update(Gymer& agent, float deltaTime) override
	{
		agent.ResetEnergy();
		if (!agent.IsEnoughHour())
		{
			agent.ChangeState(GymerStates::GoToGym);
		}
		else if (agent.IsEnoughHour())
		{
			agent.ChangeState(GymerStates::GoHomeAndSleep);
		}
	}

	void Exit(Gymer& agent) override
	{
		//empty
	}
};

class GoHomeAndSleepState : public AI::State<Gymer>
{
public:
	void Enter(Gymer& agent) override
	{
		agent.SetLocation(Gymer::LocationType::Home);
	}

	void Update(Gymer& agent, float deltaTime) override
	{
		agent.Sleeped();

		if (agent.IsSleeped())
		{
			agent.ChangeState(GymerStates::GoToGym);
		}
	}

	void Exit(Gymer& agent) override
	{
		//empty
	}
};

class GetSomeWaterState : public AI::State<Gymer>
{
public:
	void Enter(Gymer& agent) override
	{
		agent.SetLocation(Gymer::LocationType::WaterDispenser);
	}

	void Update(Gymer& agent, float deltaTime) override
	{
		agent.ResetThirst();
		if (!agent.IsThirsty())
		{
			agent.ChangeState(GymerStates::GoToGym);
		}
	}

	void Exit(Gymer& agent) override
	{
		//empty
	}
};

