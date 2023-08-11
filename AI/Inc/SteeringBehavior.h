#pragma once
#include "XEngine.h"

namespace AI 
{
	class Agent;

	class SteeringBehavior
	{
	public:
		virtual ~SteeringBehavior() = default;

		virtual X::Math::Vector2 Calculator(Agent& agent) = 0;

		void SetWeight(float weight) { mWeight = weight; }
		float GetWeight() { return mWeight; }

		void SetActive(bool active) { mActive = active; }
		bool IsActive() { return mActive; }

		void ShowDebug(bool debug) { mDebug = debug; }
		bool IsDebug() { return mDebug; }

	private:
		float mWeight = 1.0f;
		bool mActive = false;
		bool mDebug = false;
	};
}