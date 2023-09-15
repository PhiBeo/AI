#pragma once

#include <AI.h>

class VisualSensor;

class Peon : public AI::Agent
{
public:
	Peon(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ShowDebug(bool debug);

	void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
	void SetWander(bool active) { mWanderBehavior->SetActive(active); }


private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	AI::WanderBehivior* mWanderBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	VisualSensor* mVisualSensor= nullptr;
	std::array<X::TextureId, 16> mTextureIds;
};