#pragma once

#include "AI.h"

class Home : public AI::Agent
{
public:
	Home(AI::AIWorld& world);

	void Load();

	void Render();


private:
	X::TextureId mTextureId;
};