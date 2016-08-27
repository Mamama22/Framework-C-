#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "Entity.h"


/*************************************************************
Gridmap

How to use:
1) call init and pass in relevant info, Render_GridMap will be created as a child comp.

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
	int** gridmap;
	float tileScale;
	Render_GridMap* gridMap;

public:

	/******************** Constructor/destructor **********************/
	GridMap();
	~GridMap();

	/******************** Addind/removing child/comp, overload for customisation **********************/

	/******************** Core functions **********************/
	void Init(Vector3 pos, float tileScale, int totalX_tiles, int totalY_tiles);

	//Will be called everytime BEFORE comp updates-------------------//
	void Update();
};

#endif