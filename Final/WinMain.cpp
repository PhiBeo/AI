#include "ImGui/Inc/imgui.h"
#include <XEngine.h>

#include "Food.h"

AI::AIWorld aiWorld;
std::unique_ptr<Food> foodFarm;

void GameInit()
{
	aiWorld.Initialize();
	foodFarm = std::make_unique<Food>(aiWorld);
	foodFarm->Load();
}

bool GameLoop(float deltaTime)
{
	

	foodFarm->Render();

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Running Around");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}