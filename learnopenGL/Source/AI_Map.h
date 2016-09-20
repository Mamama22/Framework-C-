#ifndef AI_MAP_H
#define AI_MAP_H
#include "Entity.h"

struct XY_grid
{
	int x;
	int y;

	XY_grid(){ x = y = -1; }
	~XY_grid(){}

	XY_grid& operator=(XY_grid& copyMe)
	{
		x = copyMe.x;
		y = copyMe.y;
		return *this;
	}
};

/*************************************************************
BFS grid

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
A* node

Author: Tan Yie Cher
Date: 18/9/2016
/*************************************************************/
struct AS_Node
{
	int x, y;
	unsigned G, H, F;
	int dirX, dirY;

	AS_Node();
	~AS_Node();

	//init------------------------------------------------//
	void Init(int x, int y, bool is_wall);

	//Start path-finding----------------------------------------------//
	void Set_asStart(AS_Node& endNode);

	//Runtime----------------------------------------------//
	void Set_AsCentral(int endX, int endY);	//set as central point
	void Reset();

	void Set_G(unsigned cost);
	void Set_H(unsigned cost);
	void SetGH(unsigned G, unsigned H);
	void CalculateSurroundings(AS_Node nodeList[]);

	//flags-------------------------------------------------//
	bool isWall();
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

	/************************************* A* Path-Finding ***************************************/
	AS_Node** nodeList;	//2D array of nodes

	/************************************* internal functions ***************************************/
	void Init_AStar();
	void Init_BFS();

	void onClick_BFS();
	bool Add_AjacentTo_Frontier(BFS_Grid* addMe);
	void Display_OptimalPath_BFS();

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
	bool findPath(AI_Comp* AI_comp, const int startPt_X, const int startPt_Y, const int endPt_X, const int endPt_Y);
	void AddPath_ToDisplay(AI_Comp* AI_comp);	

	//A-Star-----------------------------------------------------//
	void Start_AStar(int startX, int startY, int endX, int endY);
	void Clear_AStar();

	//BFS--------------------------------------------------------//
	void Start_BFS(int startX, int startY, int endX, int endY);
	void Clear_BFS();

	/************************************* Get functions ***************************************/
	Vector3 Get_PointPos(int x, int y);
	int getTotal_tilesX();
	int getTotal_tilesY();
	float Get_tileScale();
};


#endif