#include "GridMap.h"

GridMap::GridMap(){}
GridMap::~GridMap(){}

/********************************************************************************
init
********************************************************************************/
void GridMap::Init(Vector3 pos, TEXTURE_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, int totalX_grids, int totalY_grids)
{
	Entity::Init(pos, Vector3(1.f, 1.f, 1.f));

	//total tiles combined for entire map---------------------------------//
	this->totalX_tiles = totalX_tiles;
	this->totalY_tiles = totalY_tiles;

	this->tileScale = tileScale;

	//total grids for entire map---------------------------------------//
	this->totalX_grids = totalX_grids;
	this->totalY_grids = totalY_grids;

	//total tiles per grid--------------------------------------------//
	int totalX_Tiles_perGrid = totalX_tiles / totalX_grids;
	int totalY_Tiles_perGrid = totalY_tiles / totalY_grids;
	Vector3 startPos = pos;

	//load gridmesh for each grid-------------------------------------//
	for (int x = 0; x < totalX_grids; ++x)
	{
		for (int y = 0; y < totalY_grids; ++y)
		{
			gridMap.push_back(new Render_GridMap());
			gridMap.back()->Init("d", tilemesh, startPos, tileScale, totalX_Tiles_perGrid, totalY_Tiles_perGrid);
			gridMap.back()->SetActive(true);
			//gridMap[(x * totalY_grids) + y]
			//gridMap.back()->SetActive(true);
			AddComponent(gridMap.back());

			startPos.y += totalY_Tiles_perGrid * tileScale;
		}

		//reset--------------------------------------//
		startPos.y = pos.y;
		startPos.x += totalX_Tiles_perGrid * tileScale;
	}
}

/********************************************************************************
init
********************************************************************************/
void GridMap::Update()
{

}