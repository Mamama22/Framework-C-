#ifndef AI_COMP_H
#define AI_COMP_H
#include "Component.h"

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
Author: Tan Yie Cher

AI component, must ATTACH to a AI_Map entity

How to use:
1) Attach to a AI_Map entity
2) Request a path from point A to B
3) AI comp will give entity a target pos. to go to, up to entity to decide how
4) when reached said point, AI comp will give another target pos. and so on


!!!!!!!!!!!!!!!!!!!!!!!!!
Can only be used in: Stage 3
points is used last to first (size - 1 --> 0) due to BFS usage

Date: 17/9/2016
/*************************************************************/
class AI_Comp : public Component
{
public:

	enum STATE
	{
		IDLE,
		PATH_FOLLOWING,
	};

private:

	int AI_Map_id;
	STATE state;
	bool display_path;

	//AI_Map info-------------//
	int totalTiles_X;
	int totalTiles_Y;
	float tileScale;

	/**************** Path-finding var *******************/
	vector<XY_grid> points;
	int pathSize;	//how many points path currently have
	int target_index;	//the index of current target point
	

public:

	/******************** constructor/destructor **********************/
	AI_Comp();
	AI_Comp(const AI_Comp& copyMe);
	~AI_Comp();

	/************************** Core functions ****************************/
	void Init(const char* name, int AI_Map_id, bool display_path);

	void Exit();

	bool findPath(int startPt_X, int startPt_Y, int endPt_X, int endPt_Y);

	/************************** target ****************************/
	Vector3 GetTargetPointPos();

	//flags-------------------//
	void GetNextPoint();	
	bool Reached_Dest();

	//settings----------------//
	void RemovePath();

	/************************** For AI Map ****************************/
	void AddPoint(int x, int y, int index);

	/************************** Get set ****************************/
	int Get_currentPathSize();
	int Get_target_index();
	int Get_xPoint(int index);
	int Get_yPoint(int index);
	float Get_tileScale();
	vector<XY_grid>& Get_XY_Grids();

	void Set_PathSize(int s);
};

#endif