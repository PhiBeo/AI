#include "ImGui/Inc/imgui.h"
#include <XEngine.h> // <> for external includes, "" for internal includes
#include "AI.h"
#include "Peon.h"

//--------------------------------------------------

AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;

bool showDebug = true;

void SpawnPeon()
{
	auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));
	peon->Load();
	peon->ShowDebug(showDebug);

	const float ScreenWidth = X::GetScreenWidth();
	const float ScreenHeight = X::GetScreenHeight();
	peon->position = X::RandomVector2({ 100.f,100.f }, { ScreenWidth - 100, ScreenHeight - 100.f});

}

void KillPeon()
{
	auto& peon = peons.back();
	peon->Unload();
	peons.pop_back();
}

void GameInit()
{
	aiWorld.Initialize();
	SpawnPeon();
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Spawn"))
	{
		SpawnPeon();
	}
	if (ImGui::Button("Kill"))
	{
		KillPeon();
	}
	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//ImGui::DragFloat("Radius", &wanderRadius, .1f, .1f, 10.f);
		//ImGui::DragFloat("Distance", &wanderRadius, .1f, .1f, 10.f);
		//ImGui::DragFloat("Jitter", &wanderJitter, .1f, .1f, 10.f);
	}

	ImGui::End();

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		const auto mouseX = static_cast<float>(X::GetMouseScreenX());
		const auto mouseY = static_cast<float>(X::GetMouseScreenY());
		const auto destination = X::Math::Vector2(mouseX, mouseY);

		for (auto& peon : peons)
		{
			peon->destination = destination;
		}
	}

	aiWorld.Update();
	for (auto& peon : peons)
	{
		peon->Update(deltaTime);
	}
	for (auto& peon : peons)
	{
		peon->Render();
	}

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	for (auto& peon : peons)
	{
		peon->Unload();
	}
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Steering");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}