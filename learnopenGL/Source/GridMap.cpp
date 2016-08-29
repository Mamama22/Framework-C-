#include "GridMap.h"
#include "CoreUtilities.h"

GridMap::GridMap(){}
GridMap::~GridMap(){}

/********************************************************************************
init
********************************************************************************/
void GridMap::Init(Vector3 pos, TEXTURE_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, 
	int totalX_grids, int totalY_grids, int tileMap_sizeX, int tileMap_sizeY, int total_SP_X, int total_SP_Y)
{
	Entity::Init(pos, Vector3(1.f, 1.f, 1.f));

	//tilemap size---------------------------------------------//
	this->tileMap_sizeX = tileMap_sizeX;
	this->tileMap_sizeY = tileMap_sizeY;

	//total tiles combined for entire map---------------------------------//
	this->totalX_tiles = totalX_tiles;
	this->totalY_tiles = totalY_tiles;

	this->tileScale = tileScale;

	//total grids for entire map---------------------------------------//
	this->totalX_grids = totalX_grids;
	this->totalY_grids = totalY_grids;

	//total tiles per grid--------------------------------------------//
	totalX_Tiles_perGrid = totalX_tiles / totalX_grids;
	totalY_Tiles_perGrid = totalY_tiles / totalY_grids;
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


	//Spartial partition-------------------------------------------//
	this->total_SP_X = total_SP_X;
	this->total_SP_Y = total_SP_Y;
	float mapScaleX = tileScale * totalX_tiles;
	float mapScaleY = tileScale * totalY_tiles;

	//X sp lines-------------------------------------------------------------//
	Render_Repetitive* render_line = new Render_Repetitive();
	render_line->Init("SD", CU::shared.quad_start0, pos, Vector3(mapScaleX, 2, 1), Vector3(0, mapScaleY / (float)total_SP_Y, 0), total_SP_X);
	render_line->SetActive(true);
	AddComponent(render_line);
	
	//Y sp lines-------------------------------------------------------------//
	render_line = new Render_Repetitive();
	render_line->Init("SD", CU::shared.quad_start0, pos, Vector3(2, mapScaleY, 1), Vector3(mapScaleX / (float)total_SP_X, 0, 0), total_SP_Y);
	render_line->SetActive(true);
	AddComponent(render_line);
}

/********************************************************************************
Update
********************************************************************************/
void GridMap::PreUpdate()
{
	Entity::PreUpdate();
}

void GridMap::Update()
{
	Entity::Update();
}

/********************************************************************************
Modify tile
********************************************************************************/
void GridMap::ModifyTile(int tileType, int x, int y)
{
	//Get the to-be modified grid---------------------------//
	int xGrid = x / totalX_Tiles_perGrid;
	int yGrid = y / totalY_Tiles_perGrid;

	Render_GridMap* modified = gridMap[(xGrid * totalY_grids) + yGrid];

	//check if is added to modified meshes list---------------------//
	bool add = true;
	for (int i = 0; i < meshModified.size(); ++i)
	{
		if (meshModified[i] == modified)
		{
			add = false;
			break;
		}
	}

	//add to modified meshes list-------------------------------//
	if (add)
		meshModified.push_back(modified);

	//modify the tile in THIS grid----------------------------------//
	int tileX = x - (totalX_Tiles_perGrid * xGrid);
	int tileY = y - (totalY_Tiles_perGrid * yGrid);

	modified->ModifyTile(tileX, tileY, tileType, tileMap_sizeX, tileMap_sizeY);
}

/********************************************************************************
Recalculate Mesh
********************************************************************************/
void GridMap::RecalculateMesh()
{
	for (int i = 0; i < meshModified.size(); ++i)
	{
		meshModified[i]->RecalculateMesh();
	}
	meshModified.clear();
}

/********************************************************************************
Getters
********************************************************************************/
int GridMap::Get_TotalGridsX(){ return totalX_grids; }
int GridMap::Get_TotalGridsY(){ return totalY_grids; }
int GridMap::Get_TilesPerGridX(){ return totalX_Tiles_perGrid; }
int GridMap::Get_TilesPerGridY(){ return totalY_Tiles_perGrid; }
int GridMap::Get_TotalTilesX(){ return totalX_tiles; }
int GridMap::Get_TotalTilesY(){ return totalY_tiles; }
int GridMap::Get_TilemapSizeX(){ return tileMap_sizeX; }
int GridMap::Get_TilemapSizeY(){ return tileMap_sizeY; }
int GridMap::Get_TilemapSize(){ return tileMap_sizeX * tileMap_sizeY; }
float GridMap::Get_TileScale(){ return tileScale; }