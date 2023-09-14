#include "ImGui/Inc/imgui.h"
#include <XEngine.h> // <> for external includes, "" for internal includes
#include "AI.h"
#include "Peon.h"
#include "TypeIds.h"

//--------------------------------------------------

AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;

bool showDebug = true;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

AI::ArriveBehavior::Deceleration deceleration = AI::ArriveBehavior::Deceleration::Normal;

int activeBehavior = 0;

void SpawnPeon()
{
	auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));
	peon->Load();
	peon->ShowDebug(showDebug);

	const float ScreenWidth = X::GetScreenWidth();
	const float ScreenHeight = X::GetScreenHeight();
	peon->position = X::RandomVector2({ 100.f,100.f }, { ScreenWidth - 100.f, ScreenHeight - 100.f});

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
	if (ImGui::Checkbox("ShowDebug", &showDebug))
	{
		for (auto& peon : peons)
		{
			peon->ShowDebug(showDebug);
		}
	}

	const static char* behaviors[] =
	{
		"Flee",
		"Seek",
		"Wander",
		"Arrive"
	};

	if (ImGui::Combo("ActiveBehavior##", &activeBehavior, behaviors, std::size(behaviors)))
	{
		for (auto& peon : peons)
		{
			peon->SetFlee(activeBehavior == 0);
			peon->SetSeek(activeBehavior == 1);
			peon->SetWander(activeBehavior == 2);
			peon->SetArrive(activeBehavior == 3);
		}
	}

	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Radius##", &wanderRadius, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Distance##", &wanderDistance, 0.1f, 0.1f, 500.f);
		ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.f);
	}

	if (ImGui::CollapsingHeader("Arrive##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const static char* decelerationSpeeds[] =
		{
			"Fast",
			"Normal",
			"Slow"
		};

		int decel = static_cast<int>(deceleration);
		if (ImGui::Combo("Deceleration##", &decel, decelerationSpeeds, std::size(decelerationSpeeds)))
		{
			deceleration = static_cast<AI::ArriveBehavior::Deceleration>(decel);
		}
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
		auto neighbors = aiWorld.GetEntitiesInRange({ peon->position, 100.0f }, Types::PeonId);
		peon->neighbors.clear();
		for (auto& n : neighbors)
		{
			if (n != peon.get())
			{
				peon->neighbors.push_back(static_cast<AI::Agent*>(n));
			}
		}
	}
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