#include "ImGui/Inc/imgui.h"
#include <XEngine.h>

#include "Gymer.h"

Gymer gymer;

void GameInit()
{
	gymer.Initialize();
}

bool GameLoop(float deltaTime)
{
	gymer.Update(deltaTime);

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	gymer.Terminate();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("FSM");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}