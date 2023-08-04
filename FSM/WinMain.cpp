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
	if(X::IsKeyPressed(X::Keys::SPACE))
		gymer.Update(deltaTime);

	ImGui::Begin("StateMachine");

	gymer.DebugUI();
	ImGui::Text("Stat: Energy [%d], Thirst [%d], Workout Time [%d]", gymer.GetEnergy(), gymer.GetThirst(), gymer.GetWorkoutTime());

	ImGui::End();

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