#ifndef AI_MAP_H
#define AI_MAP_H
#include "Entity.h"
#include <queue>
using std::queue;

/*************************************************************
BFS grid

BFS algorithm: no need 8 ajacent nodes, we can simply go diagonal by cutting thru zig-zag tiles

Author: Tan Yie Cher
Date: 19/9/2016
/*************************************************************/
struct BFS_Grid
{
	XY_grid pos;	//2D index (for keeping track of it's 2D index in 1D frontier)
	bool visited;
	bool isWall;
	XY_grid came_from;


	BFS_Grid();
	~BFS_Grid();

	void Init(int x, int y, bool isWall);

	void Reset();
	void Set_CameFrom(XY_grid came_pos);

	void SetAs_Visited();
};

/*************************************************************
AI Map

How to use:
1) Create, call init
2) pass in gridmap of obstacles
3) Set as child of a gridmap

NOTE:
Init pathfind start/end renderers in A* Init() for now
BFS: change vector<BFS_Grid*> tp BFS_Grid array for better performance

OBSERVATIONS (25/9/16):
BFS sarch with 25x25 map drop fps to 55, 100000 zombies using it drops fps to 11
Conclusion: BFS ftw

Author: Tan Yie Cher
Date: 10/9/2016
/*************************************************************/
class AI_Map : public Entity
{
private:

	enum STATE
	{
		SHOW_BFS,
		SHOW_A_STAR,
		IDLE,	//wait for components to request path
	};

	/************************************* General ***************************************/
	int** pathMap;	//DO NOT modify, local reference of the scene

	Render_GridMap* pathMap_renderer;	//highlights walkable paths
	int total_tiles_X;
	int total_tiles_Y;
	int total_passable_tiles;	//total tiles that are not obstacles
	float tileScale;

	STATE state;

	//pathfinding tools-----------------------//
	int pathFind_startX, pathFind_startY;
	int pathFind_endX, pathFind_endY;
	int pathFind_step;	//current step
	Render_InWorld* AS_start;
	Render_InWorld* AS_end;

	/************************************* BFS ***************************************/
	BFS_Grid** bfs_GridList;
	vector<BFS_Grid*> frontier;

	int frontier_lastIndex;	//keeps track of last grid of frontier

	/************************************* internal functions ***************************************/
	void Init_BFS();

	void Start_BFS(int startX, int startY, int endX, int endY);
	void onClick_BFS();
	bool Update_BFS();
	bool Add_AjacentTo_Frontier(BFS_Grid* addMe);
	void Display_OptimalPath_BFS();
	int GetOptimal_path_BFS(vector<XY_grid>& pathList);

	//utilities--------------------------------//
	void StartPathFind(int startX, int startY, int endX, int endY);	//a path-finding is initiated

public:

	AI_Map();
	~AI_Map();

	/************************************* Core functions ***************************************/
	void Init(Vector3 pos, vector< vector<int> >& pathMap, float tileScale, int total_tilesX, int totalTiles_Y);
	void Update_Stage1();
	void Update_Steps();	//update pathfind

	/************************************* Pathfinding functions ***************************************/
	int findPath(AI_Comp* AI_comp, const int startPt_X, const int startPt_Y, const int endPt_X, const int endPt_Y);
	void AddPath_ToDisplay(AI_Comp* AI_comp);	
	void Clear_Display();

	//BFS--------------------------------------------------------//
	void Start_BFS_display(int startX, int startY, int endX, int endY);

	/************************************* Get functions ***************************************/
	Vector3 Get_PointPos(int x, int y);
	int getTotal_tilesX();
	int getTotal_tilesY();
	float Get_tileScale();
};


#endif