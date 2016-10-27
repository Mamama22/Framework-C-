#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "Layer.h"
#define MAX_ENTITY_PER_GRID 50

class SP_Grid
{
private:

	int entityIntersected[MAX_ENTITY_PER_GRID];	//list of entities intersecting this grid
	int entityCount;	//how many entities intersected this grid

public:

	SP_Grid();
	SP_Grid(const SP_Grid&);
	~SP_Grid();

	void AddEntity(int handle);
	void RemoveEntity(int handle);
	void ClearAll();

	int* GetEntityList();
	int Get_entityCount();
};


/*************************************************************
Gridmap

How to use:
1) call init and pass in relevant info, Render_GridMaps will be created as child components
2) The gridmap will be seperated into grids as specifed by totalX_grids and totalY_grids

REMOVED: Children will only be added if it has a SP comp

NOTE:
-Make sure no spartial partition bound for your entity is larger than a SP grid
-Improve render order (SP)

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

	
	/******************** Path **********************/
	vector< vector<int> > pathMap;

	/******************** Spartial partition **********************/
	int total_SP_X;
	int total_SP_Y;

	IntersectedPoints current_storePoints[4];
	IntersectedPoints prev_storePoints[4];	//points from previous frame

	SP_Comp::INTERSECT_TYPE prev_intersect_type;
	SP_Comp::INTERSECT_TYPE intersect_type;

	vector<SP_Comp*> spComp_List;	//list of SP comp of children
	Render_GridMap* intersectedGrid;	//highlights intersected grid
	Vector3 sp_GridScale;

	vector<SP_Grid> SP_Grids;	//list of SP grids

	bool added_debug_SP;	//have we added debug SP renderers?

	//utilities
	void RemoveSP_FromAllGrids(SP_Comp* removeMe, IntersectedPoints prev_storePoints[]);
	void AddSP_ToGrids(SP_Comp* addMe, IntersectedPoints  current_storePoints[]);
	void Add_DebugSP_renderers();

public:

	/******************** Constructor/destructor **********************/
	GridMap();
	~GridMap();

	/******************** Core functions **********************/
	void Init(Vector3 pos, TILEMAP_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, int total_SP_X, int total_SP_Y);

	//Will be called everytime BEFORE comp updates-------------------//
	void PreUpdate();
	void Update_Stage2();

	/***************************** Addind/removing child/comp, overload for customisation *****************************/
	void AddChildren(Entity* child);

	/******************** Modify tiles **********************/
	

	/******************** Spartial Partition **********************/
	void Update_SP(Entity* updateMe);

	/******************** Getter **********************/
	int Get_TotalTilesX();
	int Get_TotalTilesY();
	float Get_TileScale();
};

#endif