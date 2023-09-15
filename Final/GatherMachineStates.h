#pragma once

#include "AI.h"

#include "GatherMachine.h"

using namespace AI;

class GoAndFindRawFoodState : public AI::State<GatherMachine>
{
public:
	void Enter(GatherMachine& agent) override
	{
		agent.SetSeek(false);
		agent.SetWander(true);
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		if (agent.HasFoundFood())
		{
			agent.SetWander(false);
			agent.SetSeek(true);
			agent.destination = agent.GetFarmPos();
		}

		if (agent.HasReachFarm())
		{
			agent.ChangeState(GatherMachineStates::GatheringRawFood);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		
	}

	void Exit(GatherMachine& agent) override
	{
		
	}
};

class GatheringRawFoodState : public AI::State<GatherMachine>
{
public:
	void Enter(GatherMachine& agent) override
	{
		agent.SetSeek(false);
		agent.SetWander(false);
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		agent.currentFood += deltaTime;

		if (agent.EnoughFood())
		{
			agent.ChangeState(GatherMachineStates::GoBackToFactory);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{

	}

	void Exit(GatherMachine& agent) override
	{

	}
};

class GoBackToFactoryState : public AI::State<GatherMachine>
{
public:
	void Enter(GatherMachine& agent) override
	{
		agent.destination = agent.GetHomePos();
		agent.SetSeek(true);
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		if (agent.IsHome())
		{
			agent.ChangeState(GatherMachineStates::ProcessRawFood);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{

	}

	void Exit(GatherMachine& agent) override
	{

	}
};

class ProcessRawFoodState : public AI::State<GatherMachine>
{
public:
	void Enter(GatherMachine& agent) override
	{
		agent.SetSeek(false);
		agent.SetWander(false);
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		agent.currentFood -= deltaTime * 2.f;
		if (agent.currentFood <= 0)
		{
			agent.ChangeState(GatherMachineStates::GoAndFindRawFood);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{

	}

	void Exit(GatherMachine& agent) override
	{

	}
};

class FleeFromChaserState : public AI::State<GatherMachine>
{
public:
	void Enter(GatherMachine& agent) override
	{
		agent.SetSeek(false);
		agent.SetWander(false);
		agent.SetFlee(true);
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		if (!agent.IsNearChaser())
		{
			if (agent.currentFood <= 0)
				agent.ChangeState(GatherMachineStates::GoAndFindRawFood);
			else
				agent.ChangeState(GatherMachineStates::GoBackToFactory);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{

	}

	void Exit(GatherMachine& agent) override
	{

	}
};