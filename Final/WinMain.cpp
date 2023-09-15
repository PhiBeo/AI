#include "ImGui/Inc/imgui.h"
#include <XEngine.h>

#include "Food.h"
#include "Home.h"
#include "GatherMachine.h"
#include "ChaseMachine.h"

AI::AIWorld aiWorld;
std::unique_ptr<Food> foodFarm;
std::unique_ptr<Home> home;
std::unique_ptr<Home> chaserHome;
std::unique_ptr<GatherMachine> gatherBot;
std::unique_ptr<ChaseMachine> chaseBot;

bool showDebug = false;

void GameInit()
{
	aiWorld.Initialize();
	foodFarm = std::make_unique<Food>(aiWorld);
	foodFarm->Load();
	home = std::make_unique<Home>(aiWorld);
	home->Load();
	chaserHome = std::make_unique<Home>(aiWorld);
	chaserHome->Load();
	gatherBot = std::make_unique<GatherMachine>(aiWorld);
	gatherBot->Load();
	gatherBot->SetHome(home->position);
	chaseBot = std::make_unique<ChaseMachine>(aiWorld);
	chaseBot->Load();
	chaseBot->SetHome(chaserHome->position);
	gatherBot->target = chaseBot.get();
}

bool GameLoop(float deltaTime)
{
	home->Render();
	chaserHome->Render();

	foodFarm->Update(deltaTime);
	foodFarm->Render();

	gatherBot->Update(deltaTime);
	gatherBot->Render();

	chaseBot->Update(deltaTime);
	chaseBot->Render();

	ImGui::Begin("Setting");

	gatherBot->GetStateMachine()->DebugUI();
	chaseBot->GetStateMachine()->DebugUI();

	ImGui::Checkbox("Check Debug", &showDebug);

	ImGui::End();


	gatherBot->ShowDebug(showDebug);


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