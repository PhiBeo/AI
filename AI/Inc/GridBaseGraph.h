#pragma once


namespace AI
{
	class GridBaseGraph
	{
	public:
		enum Direction
		{
			South,
			North,
			East,
			West,
			NorthEast,
			NorthWest,
			SouthEast,
			SouthWest
		};

		struct Node
		{
			std::array<Node*, 8> neighbors = {};
			int column = 0, row = 0;

			Node* parent = nullptr;
			bool opened = false;
			bool closed = false;

			float h = 0, g = 0;

			void Reset()
			{
				parent = nullptr;
				opened = false;
				closed = false;
				h = 0;
				g = 0;
			}
		};

		void Initialize(int columns, int rows);

		void ResetSearchParams();

		Node* GetNode(int x, int y);
		const Node* GetNode(int x, int y) const;

		int GetColumns() const { return mColumns; }
		int GetRows() const { return mRows; }

		float GetHeuristics(int startX, int startY, int endX, int endY);

	private:
		int GetIndex(int x, int y) const;

		std::vector<Node> mNodes;
		int mColumns = 0;
		int mRows = 0;
	};

	
}