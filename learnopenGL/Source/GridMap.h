#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "Entity.h"


/*************************************************************
Gridmap

How to use:
1) call init and pass in relevant info, Render_GridMaps will be created as child components
2) The gridmap will be seperated into grids as specifed by totalX_grids and totalY_grids

NOTE:
-Make sure no spartial partition bound for your entity is larger than a SP grid

Author: Tan Yie Cher
Date: 24/8/2016
/*************************************************************/
class GridMap : public Entity
{
	/******************** Enums **********************/

protected:

	/******************** shared **********************/
	int totalX_tiles;
	int totalY_tiles;

	//tiles of all the grid-----------------//
	float tileScale;

	/******************** Render **********************/
	//grids-----------------------------//
	int totalX_grids;
	int totalY_grids;

	int totalX_Tiles_perGrid;
	int totalY_Tiles_perGrid;

	//tilemap--------------------------//
	int tileMap_sizeX;
	int tileMap_sizeY;
	
	vector<Render_GridMap*> gridMap;


	//modifying tools--------------------------//
	vector<Render_GridMap*> meshModified;

	/******************** Spartial partition **********************/
	int total_SP_X;
	int total_SP_Y;

public:

	/******************** Constructor/destructor **********************/
	GridMap();
	~GridMap();

	/******************** Core functions **********************/
	void Init(Vector3 pos, TEXTURE_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, 
		int totalX_grids, int totalY_grids, int tileMap_sizeX, int tileMap_sizeY, int total_SP_X, int total_SP_Y);

	//Will be called everytime BEFORE comp updates-------------------//
	void PreUpdate();
	void Update();

	/******************** Modify tiles **********************/
	void ModifyTile(int tileType, int x, int y);
	void RecalculateMesh();	//call after modifying tiles

	/******************** Spartial Partition **********************/
	void Update_SP(Entity* updateMe);

	/******************** Getter **********************/
	int Get_TotalGridsX();
	int Get_TotalGridsY();
	int Get_TilesPerGridX();
	int Get_TilesPerGridY();
	int Get_TotalTilesX();
	int Get_TotalTilesY();
	int Get_TilemapSizeX();
	int Get_TilemapSizeY();
	int Get_TilemapSize();
	float Get_TileScale();
};

#endif