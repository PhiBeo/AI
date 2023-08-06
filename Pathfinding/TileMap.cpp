#include "TileMap.h"

using namespace AI;

namespace
{
	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);
	int count = 0;
	std::string buffer;

	file >> buffer;
	file >> count;

	mTiles.clear();
	mTiles.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		bool isBlocked = 0;
		file >> buffer;
		file >> isBlocked;

		auto& tile = mTiles.emplace_back();
		tile.isBlock = isBlocked == 1;
		tile.textureId = X::LoadTexture(buffer.c_str());
	}

	file.close();

	mTileWidth = X::GetSpriteWidth(mTiles[0].textureId);
	mTileHeight = X::GetSpriteHeight(mTiles[0].textureId);
}

void TileMap::LoadMap(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);
	int columns = 0, rows = 0;
	std::string buffer;

	file >> buffer;
	file >> columns;
	file >> buffer;
	file >> rows;

	mColumns = columns;
	mRows = rows;

	mMap.resize(columns * rows);
	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			char tileType;
			file >> tileType;
			mMap[ToIndex(x, y, columns)] = tileType - '0';
		}
	}

	file.close();

	auto GetNeighbor = [&](int c, int r) -> AI::GridBaseGraph::Node*
	{
		if (IsBlock(c, r))
		{
			return nullptr;
		}
		return mGraph.GetNode(c, r);
	};

	mGraph.Initialize(mColumns, mRows);
	for (int r = 0; r < mRows; ++r)
	{
		for (int c = 0; c < mColumns; ++c)
		{
			if (IsBlock(c, r))
			{
				continue;
			}

			GridBaseGraph::Node* node = mGraph.GetNode(c, r);
			node->neighbors[GridBaseGraph::Direction::North] = GetNeighbor(c, r - 1);
			node->neighbors[GridBaseGraph::Direction::South] = GetNeighbor(c, r + 1);
			node->neighbors[GridBaseGraph::Direction::West] = GetNeighbor(c - 1, r);
			node->neighbors[GridBaseGraph::Direction::East] = GetNeighbor(c + 1, r);
			node->neighbors[GridBaseGraph::Direction::NorthEast] = GetNeighbor(c + 1, r - 1);
			node->neighbors[GridBaseGraph::Direction::NorthWest] = GetNeighbor(c - 1, r - 1);
			node->neighbors[GridBaseGraph::Direction::SouthEast] = GetNeighbor(c + 1, r + 1);
			node->neighbors[GridBaseGraph::Direction::SouthWest] = GetNeighbor(c - 1, r + 1);
		}
	}
}

void TileMap::Render() const
{
	float spriteWidth = X::GetSpriteWidth(mTiles[0].textureId);
	float spriteHeight = X::GetSpriteHeight(mTiles[0].textureId);
	X::Math::Vector2 pos;

	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int index = ToIndex(x, y, mColumns);
			X::DrawSprite(mTiles[mMap[index]].textureId, pos, X::Pivot::TopLeft);
			pos.x += spriteWidth;
		}
		pos.x = 0.0f;
		pos.y += spriteHeight;
	}

	for (int r = 0; r < mRows; r++)
	{
		for (int c = 0; c < mColumns; c++)
		{
			const auto node = mGraph.GetNode(c, r);
			for (const auto neighbor : node->neighbors)
			{
				if (neighbor == nullptr)
					continue;

				const auto a = GetPixelPosition(node->column, node->row);
				const auto b = GetPixelPosition(neighbor->column, neighbor->row);
				X::DrawScreenLine(a, b, X::Colors::DarkGray);
			}
		}
	}
}

bool TileMap::IsBlock(int x, int y) const
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		int index = ToIndex(x, y, mColumns);
		if (index < mMap.size())
		{
			int tile = mMap[index];
			return mTiles[tile].isBlock;
		}
		return true;
	}
}

Path TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
	Path path;
	BFS bfs;
	if (bfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closeList = bfs.GetCloseList();
		auto node = closeList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
	Path path;
	DFS dfs;
	if (dfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closeList = dfs.GetCloseList();
		auto node = closeList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
	Path path;
	Dijkstra dijkstra;
	auto getCost = [](const GridBaseGraph::Node* node, const GridBaseGraph::Node* neighbor)
	{
		if (node->column != neighbor->column && node->row != neighbor->row)
		{
			return 1.5f;
		}
		return 1.0f;
	};

	if (dijkstra.Run(mGraph, startX, startY, endX, endY, getCost))
	{
		const auto& closeList = dijkstra.GetCloseList();
		auto node = closeList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindAStar(int startX, int startY, int endX, int endY)
{
	Path path;
	AStar aStar;
	auto getCost = [](const GridBaseGraph::Node* node, const GridBaseGraph::Node* neighbor)
	{
		if (node->column != neighbor->column && node->row != neighbor->row)
		{
			return 1.5f;
		}
		return 1.0f;
	};

	auto getHeuristic = [](const GridBaseGraph::Node* neighbor, const GridBaseGraph::Node* endNode)
	{
		float D = 1.0f;
		float dx = abs(neighbor->column - endNode->column);
		float dy = abs(neighbor->row - endNode->row);
		return D * (dx + dy);
	};

	if (aStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
	{
		const auto& closeList = aStar.GetCloseList();
		auto node = closeList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

X::Math::Vector2 TileMap::GetPixelPosition(int x, int y) const
{
	return {
		(x + 0.5f) * mTileWidth,
		(y + 0.5f) * mTileHeight
	};
}

// 2D map - 5 columns x 4 rows
// [0][0][0][0][0]
// [0][0][0][0][0]
// [0][0][0][X][0]   X is at (3, 2)
// [0][0][0][0][0]

// Stored as 1D - 5x4 = 20 slots
// [0][0][0][0][0] [0][0][0][0][0] [0][0][0][X][0] [0][0][0][0][0]
//
// index = column + (row * columnCount)
//       = 3 + (2 * 5)
//       = 13