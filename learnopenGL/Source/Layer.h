#ifndef LAYER_H
#define LAYER_H
#include "Entity.h"

/*==========================================================================================================================
Gridmap layer, add to gridmap
One layer can have more than 1 grid, each grid can have 1 or more tile
Tile size and total amount DETERMINED by parent gridmap

Instructions:
1) Call Init(...)
2) Add as child to GridMap
3) Modify tiles

if modified tiles before adding as child, tile values will not be matched with GridMap


Author: Tan Yie Cher
Date: 1/10/2016
/===========================================================================================================================*/
class GridLayer : public Entity
{
	int totalX_tiles;
	int totalY_tiles;
	int totalX_grids;
	int totalY_grids;
	int totalX_Tiles_perGrid;
	int totalY_Tiles_perGrid;
	float tileScale;
	TILEMAP_ENUM tilemap_enum;

	vector<Render_GridMap*> render_gridMap;	//contain all the grid renderer
	vector<Render_GridMap*> meshModified;	//keeps track of modified renderer per session (For map design purpose only)

	/******************** utilities **********************/
	Render_GridMap* getGrid_fromCoord(int x, int y);

public:

	GridLayer();
	GridLayer(const GridLayer& copyMe);
	~GridLayer();

	/******************** comp functions **********************/
	//, float tileScale, int totalX_tiles, int totalY_tiles, int totalX_grids, int totalY_grids
	void Init(const char* name, TILEMAP_ENUM tileMap_tex, Vector3 pos);

	/******************** Core functions **********************/
	void ModifyTile(int tileType, int x, int y);
	void RecalculateMesh();	//call after modifying tiles

	/******************** Utilities functions **********************/
	void SetAllTiles(int tileType);
	void SetTileEmpty(int x, int y);
	void SetAllTilesEmpty();

	/******************** Added/removed **********************/
	void Added_toGridMap(float tileScale, int totalX_tiles, int totalY_tiles);	//if this entity added, do something

	/******************** Getter **********************/
	int Get_TotalGridsX();
	int Get_TotalGridsY();
	int Get_TilesPerGridX();
	int Get_TilesPerGridY();
	int Get_TotalTilesX();
	int Get_TotalTilesY();
	TILEMAP_ENUM Get_TilemapEnum();
	float Get_TileScale();
};

#endif