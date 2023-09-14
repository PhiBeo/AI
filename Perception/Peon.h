#pragma once

#include <AI.h>

class Peon : public AI::Agent
{
public:
	Peon(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ShowDebug(bool debug);

	void SetFlee(bool active) { mFleeBehavior->SetActive(active); }
	void SetSeek(bool active) { mSeekBehavior->SetActive(active); }
	void SetWander(bool active) { mWanderBehavior->SetActive(active); }
	void SetArrive(bool active) { mArriveBehavior->SetActive(active); }

private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehivior* mWanderBehavior = nullptr;
	AI::SeekBehavior* mSeekBehavior = nullptr;
	AI::FleeBehavior* mFleeBehavior = nullptr;
	AI::ArriveBehavior* mArriveBehavior = nullptr;
	AI::SeperationBehavior* mSeperationBehavior = nullptr;
	AI::AlignmentBehavior* mAlignmentBehavior = nullptr;
	AI::CohesionBehavior* mCohesionBehavior = nullptr;
	std::array<X::TextureId, 16> mTextureIds;
};