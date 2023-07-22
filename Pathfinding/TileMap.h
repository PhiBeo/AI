#pragma once

#include <XEngine.h>
#include <AI.h>

struct Tile
{
	X::TextureId textureId = 0;
	bool isBlock = false;
};

class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render() const;
	bool IsBlock(int x, int y) const;

private:
	X::Math::Vector2 GetPixelPosition(int x, int y) const;

	AI::GridBaseGraph mGraph;
	std::vector<int> mMap;
	std::vector<Tile> mTiles; // I forgot to add this in class
	int mColumns = 0;
	int mRows = 0;
	int mTileWidth = 0;
	int mTileHeight = 0;
};