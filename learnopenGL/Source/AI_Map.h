#ifndef AI_MAP_H
#define AI_MAP_H
#include "Entity.h"

/*************************************************************
AI Map

How to use:
1) Create, call init
2) pass in gridmap of obstacles
3) Set as child of a gridmap

NOTE:

Author: Tan Yie Cher
Date: 10/9/2016
/*************************************************************/
class AI_Map : public Entity
{
	Render_GridMap* pathMap_renderer;	//highlights walkable paths

public:

	AI_Map();
	~AI_Map();

	/******************** Core functions **********************/
	void Init(Vector3 pos, vector< vector<int> >& pathMap, float tileScale, int total_tilesX, int totalTiles_Y);
};


#endif