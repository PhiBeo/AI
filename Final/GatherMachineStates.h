#pragma once

#include "AI.h"
#include "ImGui/Inc/imgui.h"
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

		if (agent.IsReach(Types::FoodId))
		{
			agent.ChangeState(GatherMachineStates::GatheringRawFood);
		}

		if (agent.IsNearChaser())
		{
			agent.ChangeState(GatherMachineStates::FleeFromChaser);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		ImGui::Text("Gather is finding food");
		ImGui::DragFloat("food", &agent.currentFood, 0.0f, 0.0f, 50.f);
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
		agent.velocity = X::Math::Vector2::Zero();
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		agent.currentFood += deltaTime;

		if (agent.EnoughFood())
		{
			agent.ChangeState(GatherMachineStates::GoBackToFactory);
		}

		if (agent.IsNearChaser())
		{
			agent.ChangeState(GatherMachineStates::FleeFromChaser);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		ImGui::Text("Gather is gathering food");
		ImGui::DragFloat("food", &agent.currentFood, 0.0f, 0.0f, 50.f);
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
		if (agent.IsReach(Types::HomeId))
		{
			agent.ChangeState(GatherMachineStates::ProcessRawFood);
		}

		if (agent.IsNearChaser())
		{
			agent.ChangeState(GatherMachineStates::FleeFromChaser);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		ImGui::Text("Gather is going back");
		ImGui::DragFloat("food", &agent.currentFood, 0.0f, 0.0f, 50.f);
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
		agent.velocity = X::Math::Vector2::Zero();
	}

	void Update(GatherMachine& agent, float deltaTime) override
	{
		agent.currentFood -= deltaTime * 2.f;

		if (agent.currentFood <= 0)
		{
			agent.ChangeState(GatherMachineStates::GoAndFindRawFood);
		}

		if (agent.IsNearChaser())
		{
			agent.ChangeState(GatherMachineStates::FleeFromChaser);
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		ImGui::Text("Gather is processing food");
		ImGui::DragFloat("food", &agent.currentFood, 0.0f, 0.0f, 50.f);
	}

	void Exit(GatherMachine& agent) override
	{
		agent.FinishFood();
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
		agent.destination = agent.target->position;

		if (!agent.IsNearChaser())
		{
			if (agent.currentFood <= 0)
			{
				agent.ChangeState(GatherMachineStates::GoAndFindRawFood);
				agent.destination = agent.GetFarmPos();
			}
			else
			{
				agent.ChangeState(GatherMachineStates::GoBackToFactory);
				agent.destination = agent.GetHomePos();
			}
		}
	}

	void DebugUI(GatherMachine& agent) override
	{
		ImGui::Text("Gather is running");
		ImGui::DragFloat("food", &agent.currentFood, 0.0f, 0.0f, 50.f);
	}

	void Exit(GatherMachine& agent) override
	{
		agent.SetFlee(false);
	}
};