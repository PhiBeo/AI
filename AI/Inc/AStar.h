#pragma once

#include "GridBaseGraph.h"

namespace AI
{
	using NodeList = std::list<GridBaseGraph::Node*>;

	class AStar
	{
	public:
		bool Run(GridBaseGraph& graph, int startX, int startY, int endX, int endY);

		const NodeList& GetCloseList() const { return mCloseList; }

	private:
		NodeList mOpenList;
		NodeList mCloseList;
	};
}