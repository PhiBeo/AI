#pragma once

#include "GridBaseGraph.h"

namespace AI
{
	using NodeList = std::list<GridBaseGraph::Node*>;
	using GetCost = std::function<float(const GridBaseGraph::Node*, const GridBaseGraph::Node*)>;
	using GetHeuristic = std::function<float(const GridBaseGraph::Node*, const GridBaseGraph::Node*)>;


	class AStar
	{
	public:
		bool Run(GridBaseGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost, GetHeuristic getHeuristic);

		const NodeList& GetCloseList() const { return mCloseList; }

	private:
		NodeList mOpenList;
		NodeList mCloseList;
	};
}