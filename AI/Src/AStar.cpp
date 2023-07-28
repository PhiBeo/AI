#include "Precompiled.h"
#include "AStar.h"

bool AI::AStar::Run(GridBaseGraph& graph, int startX, int startY, int endX, int endY)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mCloseList.clear();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	bool found = false;

	while (!found && !mOpenList.empty())
	{
		node = mOpenList.front();
		mOpenList.pop_front();

		if (node->column == endX && node->row == endY)
		{
			found = true;
		}
		else
		{
			for (auto neighbor : node->neighbors)
			{
				if (neighbor == nullptr || neighbor->opened)
					continue;

				
			}
		}

		mCloseList.push_back(node);
		node->closed;
	}

	return found;
}
