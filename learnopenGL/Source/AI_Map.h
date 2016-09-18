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
private:

	int** pathMap;	//DO NOT modify, local reference of the scene

	Render_GridMap* pathMap_renderer;	//highlights walkable paths
	int total_tiles_X;
	int total_tiles_Y;
	float tileScale;

	/******************** Utilities **********************/

public:

	AI_Map();
	~AI_Map();

	/******************** Core functions **********************/
	void Init(Vector3 pos, vector< vector<int> >& pathMap, float tileScale, int total_tilesX, int totalTiles_Y);

	/******************** Pathfinding functions **********************/
	bool findPath(AI_Comp* AI_comp, const int startPt_X, const int startPt_Y, const int endPt_X, const int endPt_Y);
	void AddPath_ToDisplay(AI_Comp* AI_comp);	

	/******************** Get functions **********************/
	Vector3 Get_PointPos(int x, int y);
	int getTotal_tilesX();
	int getTotal_tilesY();
	float Get_tileScale();
};


#endif