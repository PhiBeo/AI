#include "Precompiled.h"
#include "GridBaseGraph.h"

using namespace AI;

void AI::GridBaseGraph::Initialize(int columns, int rows)
{
	mNodes.clear();
	mNodes.reserve(columns * rows);
	mColumns = columns;
	mRows = rows;

	for (int r = 0; r < rows; ++r)
	{
		for (int c = 0; c < columns; ++c)
		{
			auto& node = mNodes.emplace_back();
			node.column = c;
			node.row = r;
		}
	}
}

void AI::GridBaseGraph::ResetSearchParams()
{
	for (auto& node : mNodes)
	{
		node.Reset();
	}
}

GridBaseGraph::Node* GridBaseGraph::GetNode(int x, int y)
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		const int index = GetIndex(x, y);
		if (index < mNodes.size())
		{
			return &mNodes[index];
		}
	}
	return nullptr;
}

const GridBaseGraph::Node* GridBaseGraph::GetNode(int x, int y) const
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		const int index = GetIndex(x, y);
		if (index < mNodes.size())
		{
			return &mNodes[index];
		}
	}
	return nullptr;
}

float AI::GridBaseGraph::GetHeuristics(int startX, int startY, int endX, int endY)
{
	int dX = X::Math::Abs(startX - endX);
	int dY = X::Math::Abs(startY - endY);

	int D = 1;
	int D2 = X::Math::Sqrt(2);

	return D * (dX + dY) + (D2 - 2 * D) * X::Math::Min(dX, dY);
}

int GridBaseGraph::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}
