#include "Precompiled.h"
#include "SteeringModule.h"

using namespace AI;

SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{

}

X::Math::Vector2 SteeringModule::Calculator()
{
	X::Math::Vector2 totalForce;

	for (auto& behavior : mBehaviors)
	{
		if (behavior->IsActive())
		{
			totalForce += behavior->Calculator(mAgent) * behavior->GetWeight();
		}
	}
	return totalForce;
}