#pragma once

#include "AI.h"

enum Types;

class Food : public AI::Agent
{
public:
	Food(AI::AIWorld& world);

	void Load();
	void Update(float deltaTime);
	void Render();

	bool IsInteract(Types type);

private:
	X::TextureId mTextureId;
	bool wasGathered;
};