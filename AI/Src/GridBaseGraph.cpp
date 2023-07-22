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

GridBaseGraph::Node* GridBaseGraph::GetNode(int x, int y)
{
	const int index = GetIndex(x, y);
	if (index < mNodes.size())
	{
		return &mNodes[index];
	}
	return nullptr;
}

const GridBaseGraph::Node* AI::GridBaseGraph::GetNode(int x, int y) const
{
	const int index = GetIndex(x, y);
	if (index < mNodes.size())
	{
		return &mNodes[index];
	}
	return nullptr;
}

int GridBaseGraph::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}
