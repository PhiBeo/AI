#pragma once

#include "AI.h"
#include "ImGui/Inc/imgui.h"
#include "ChaseMachine.h"

using namespace AI;

class WanderAroundState : public AI::State<ChaseMachine>
{
public:
	void Enter(ChaseMachine& agent) override
	{
		agent.SetWander(true);
		agent.SetSeek(false);
		agent.wanderingTime = X::RandomFloat(5.f, 20.f);
	}

	void Update(ChaseMachine& agent, float deltaTime) override
	{
		agent.wanderingTime -= deltaTime;

		if (agent.IsDetect())
		{
			agent.ChangeState(ChaseMachineStates::ChaseGatherer);
		}
		if (agent.wanderingTime <= 0)
		{
			agent.ChangeState(ChaseMachineStates::GoHome);
		}
	}

	void DebugUI(ChaseMachine& agent) override
	{
		ImGui::Text("Chaser is wandering");
	}

	void Exit(ChaseMachine& agent) override
	{

	}
};

class GoHomeState : public AI::State<ChaseMachine>
{
public:
	void Enter(ChaseMachine& agent) override
	{
		agent.SetSeek(true);
		agent.SetWander(false);
		agent.destination = agent.GetHome();
	}

	void Update(ChaseMachine& agent, float deltaTime) override
	{
		if (agent.IsReach(Types::HomeId))
		{
			agent.ChangeState(ChaseMachineStates::Sleep);
		}
	}

	void DebugUI(ChaseMachine& agent) override
	{
		ImGui::Text("Chaser is going Home");
	}

	void Exit(ChaseMachine& agent) override
	{

	}
};

class SleepState : public AI::State<ChaseMachine>
{
public:
	void Enter(ChaseMachine& agent) override
	{
		agent.velocity = X::Math::Vector2::Zero();
		agent.SetSeek(false);
		agent.SetWander(false);
		agent.sleepTime = X::RandomFloat(3.f, 10.f);
	}

	void Update(ChaseMachine& agent, float deltaTime) override
	{
		agent.sleepTime -= deltaTime;
		if (agent.sleepTime <= 0)
			agent.ChangeState(ChaseMachineStates::WanderAround);
	}

	void DebugUI(ChaseMachine& agent) override
	{
		ImGui::Text("Chaser is sleeping");
	}

	void Exit(ChaseMachine& agent) override
	{

	}
};

class ChaseGathererState : public AI::State<ChaseMachine>
{
public:
	void Enter(ChaseMachine& agent) override
	{
		agent.SetWander(false);
		agent.SetSeek(true);
	}

	void Update(ChaseMachine& agent, float deltaTime) override
	{
		agent.destination = agent.GetDetectedPos();

		if (!agent.IsDetect())
			agent.ChangeState(ChaseMachineStates::WanderAround);
	}

	void DebugUI(ChaseMachine& agent) override
	{
		ImGui::Text("Chaser is chasing");
	}

	void Exit(ChaseMachine& agent) override
	{

	}
};