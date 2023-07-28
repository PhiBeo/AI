#include "TileMap.h"
#include "ImGui/Inc/imgui.h"
#include <XEngine.h> // <> for external includes, "" for internal includes

TileMap tileMap;
Path path;
int startX = 5, startY = 12;
int endX = 9, endY = 16;

//--------------------------------------------------

void GameInit()
{
	tileMap.LoadTiles("tiles.txt");
	tileMap.LoadMap("map.txt");
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Pathfinding");
	ImGui::DragInt("StartX##", &startX, 1, 0, tileMap.GetColumns() - 1);
	ImGui::DragInt("StartY##", &startY, 1, 0, tileMap.GetRows() - 1);
	ImGui::DragInt("EndX##", &endX, 1, 0, tileMap.GetColumns() - 1);
	ImGui::DragInt("EndY##", &endY, 1, 0, tileMap.GetRows() - 1);
	if (ImGui::Button("RunBFS##"))
	{
		path = tileMap.FindPathBFS(startX, startY, endX, endY);
	}
	if (ImGui::Button("RunDFS##"))
	{
		path = tileMap.FindPathDFS(startX, startY, endX, endY);
	}
	ImGui::End();

	tileMap.Render();

	for (int i = 1; i < path.size(); ++i)
	{
		X::DrawScreenLine(path[i - 1], path[i], X::Colors::Green);
	}

	X::DrawScreenCircle(tileMap.GetPixelPosition(startX, startY), 10, X::Colors::Yellow);
	X::DrawScreenCircle(tileMap.GetPixelPosition(endX, endY), 10, X::Colors::Blue);

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Pathfinding");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}