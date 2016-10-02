#include "Layer.h"

GridLayer::GridLayer()
{
	totalX_tiles = 0;
	totalY_tiles = 0;
	totalX_grids = 0;
	totalY_grids = 0;
	totalX_Tiles_perGrid = 0;
	totalY_Tiles_perGrid = 0;
	tileScale = 0;
}

GridLayer::GridLayer(const GridLayer& copyMe)
{
	totalX_tiles = copyMe.totalX_tiles;
	totalY_tiles = copyMe.totalY_tiles;
	totalX_grids = copyMe.totalX_grids;
	totalY_grids = copyMe.totalY_grids;
	totalX_Tiles_perGrid = copyMe.totalX_Tiles_perGrid;
	totalY_Tiles_perGrid = copyMe.totalY_Tiles_perGrid;
	tileScale = copyMe.tileScale;
}

GridLayer::~GridLayer(){}

/********************************************************************************
init
********************************************************************************/
void GridLayer::Init(const char* name, TILEMAP_ENUM tileMap_tex, Vector3 pos)
{
	Entity::Init(pos, Vector3(1.f, 1.f, 1.f));
	this->tilemap_enum = tileMap_tex;
}

/********************************************************************************
Added to parent gridmap
********************************************************************************/
void GridLayer::Added_toGridMap(float tileScale, int totalX_tiles, int totalY_tiles)
{
	this->tileScale = tileScale;

	//total tiles combined for entire map---------------------------------//
	this->totalX_tiles = totalX_tiles;
	this->totalY_tiles = totalY_tiles;

	float mapScaleX = tileScale * totalX_tiles;
	float mapScaleY = tileScale * totalY_tiles;

	//total grids for entire map---------------------------------------//
	this->totalX_grids = 1;
	this->totalY_grids = 1;

	//total grids for entire map---------------------------------------//
	this->totalX_grids = totalX_grids;
	this->totalY_grids = totalY_grids;

	//total tiles per grid--------------------------------------------//
	totalX_Tiles_perGrid = totalX_tiles / totalX_grids;
	totalY_Tiles_perGrid = totalY_tiles / totalY_grids;
	Vector3 startPos = transform.GetPos();

	//load gridmesh for each grid-------------------------------------//
	for (int x = 0; x < totalX_grids; ++x)
	{
		for (int y = 0; y < totalY_grids; ++y)
		{
			render_gridMap.push_back(new Render_GridMap());
			render_gridMap.back()->Init("d", tilemap_enum, startPos, tileScale, totalX_Tiles_perGrid, totalY_Tiles_perGrid);
			//gridMap[(x * totalY_grids) + y]
			//render_gridMap.back()->SetActive(true);
			AddComponent(render_gridMap.back());

			startPos.y += totalY_Tiles_perGrid * tileScale;
		}

		//reset--------------------------------------//
		startPos.y = transform.pos.y;
		startPos.x += totalX_Tiles_perGrid * tileScale;
	}
}

/********************************************************************************
Get the renderer based on the coords provided
Will add to modified list automatically
********************************************************************************/
Render_GridMap* GridLayer::getGrid_fromCoord(int x, int y)
{
	//Get the to-be modified grid---------------------------//
	int xGrid = x / totalX_Tiles_perGrid;
	int yGrid = y / totalY_Tiles_perGrid;

	Render_GridMap* modified = render_gridMap[(xGrid * totalY_grids) + yGrid];

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

	return modified;
}

/********************************************************************************
Modify tile
********************************************************************************/
void GridLayer::ModifyTile(int tileType, int x, int y)
{
	//Get the to-be modified grid---------------------------//
	int xGrid = x / totalX_Tiles_perGrid;
	int yGrid = y / totalY_Tiles_perGrid;

	Render_GridMap* modified = getGrid_fromCoord(x, y);

	//modify the tile in THIS grid----------------------------------//
	int tileX = x - (totalX_Tiles_perGrid * xGrid);
	int tileY = y - (totalY_Tiles_perGrid * yGrid);

	modified->SetTile(tileX, tileY, tileType);
}

/********************************************************************************
Set all tiles to the same type
********************************************************************************/
void GridLayer::SetAllTiles(int tileType)
{
	for (int x = 0; x < render_gridMap.size(); ++x)
	{
		render_gridMap[x]->SetAllTiles(tileType);
	}
}

/********************************************************************************
Set tile empty
********************************************************************************/
void GridLayer::SetTileEmpty(int x, int y)
{
	//Get the to-be modified grid---------------------------//
	int xGrid = x / totalX_Tiles_perGrid;
	int yGrid = y / totalY_Tiles_perGrid;

	Render_GridMap* modified = getGrid_fromCoord(x, y);

	//modify the tile in THIS grid----------------------------------//
	int tileX = x - (totalX_Tiles_perGrid * xGrid);
	int tileY = y - (totalY_Tiles_perGrid * yGrid);

	modified->SetTileEmpty(tileX, tileY);
}

/********************************************************************************
Set ALL tile empty
********************************************************************************/
void GridLayer::SetAllTilesEmpty()
{
	for (int x = 0; x < render_gridMap.size(); ++x)
	{
		render_gridMap[x]->SetAllTilesEmpty();
	}
}

/********************************************************************************
Recalculate Mesh
********************************************************************************/
void GridLayer::RecalculateMesh()
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
int GridLayer::Get_TotalGridsX(){ return totalX_grids; }
int GridLayer::Get_TotalGridsY(){ return totalY_grids; }
int GridLayer::Get_TilesPerGridX(){ return totalX_Tiles_perGrid; }
int GridLayer::Get_TilesPerGridY(){ return totalY_Tiles_perGrid; }
int GridLayer::Get_TotalTilesX(){ return totalX_tiles; }
int GridLayer::Get_TotalTilesY(){ return totalY_tiles; }
TILEMAP_ENUM GridLayer::Get_TilemapEnum(){ return tilemap_enum; }
float GridLayer::Get_TileScale(){ return tileScale; }
