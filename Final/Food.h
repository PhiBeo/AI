#pragma once

#include "AI.h"

class Food : public AI::Agent
{
public:
	Food(AI::AIWorld& world);

	void Load();

	void Render();


private:
	X::TextureId mTextureId;
};