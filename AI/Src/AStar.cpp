#include "Precompiled.h"
#include "AStar.h"

bool AI::AStar::Run(GridBaseGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost, GetHeuristic getHeuristic)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mCloseList.clear();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	auto endNode = graph.GetNode(endX, endY);

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
				if (neighbor == nullptr || neighbor->closed)
					continue;

				int cost = node->cost + getCost(node, neighbor);
				if (!neighbor->opened)
				{
					mOpenList.push_back(neighbor);
					neighbor->opened = true;
					neighbor->parent = node;
					neighbor->cost = cost;
					neighbor->heuristic = getHeuristic(neighbor, endNode);
				}
				else if (cost < neighbor->cost)
				{
					neighbor->parent = node;
					neighbor->cost = cost;
				}
			}
		}
		auto costSort = [](const GridBaseGraph::Node* a, const GridBaseGraph::Node* b)
		{
			return a->cost + a->heuristic < b->cost + b->heuristic;
		};

		mOpenList.sort(costSort);

		mCloseList.push_back(node);
		node->closed;
	}

	return found;
}
