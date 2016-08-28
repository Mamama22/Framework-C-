#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "Entity.h"


/*************************************************************
Gridmap

How to use:
1) call init and pass in relevant info, Render_GridMaps will be created as child components
2) The gridmap will be seperated into grids as specifed by totalX_grids and totalY_grids

Author: Tan Yie Cher
Date: 24/8/2016
/*************************************************************/
class GridMap : public Entity
{
	/******************** Enums **********************/

protected:

	/******************** Var **********************/
	int totalX_tiles;
	int totalY_tiles;

	int** tilemap;	//tilemap for all grids
	float tileScale;

	int totalX_grids;	//grids
	int totalY_grids;

	vector<Render_GridMap*> gridMap;

public:

	/******************** Constructor/destructor **********************/
	GridMap();
	~GridMap();

	/******************** Addind/removing child/comp, overload for customisation **********************/

	/******************** Core functions **********************/
	void Init(Vector3 pos, TEXTURE_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, int totalX_grids, int totalY_grids);

	//Will be called everytime BEFORE comp updates-------------------//
	void Update();
};

#endif