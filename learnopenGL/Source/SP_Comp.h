#ifndef SP_COMP_H
#define SP_COMP_H
#include "Renderer.h"

struct IntersectedPoints
{
	int x;
	int y;

	IntersectedPoints(){ SetNone(); }
	~IntersectedPoints(){}

	void SetNone(){ x = y = -1; }
	void SetPoint(int x, int y){ this->x = x; this->y = y; }
};

/*************************************************************
Author: Tan Yie Cher

Spartial partition component, makes sure each Entity only have ONE
Spartial partition will only take the first SP comp found on each Entity
TO be used with GridMap Entity

How it works:
1) Assumes parent Entity of this comp. will be added as child of the Spartial Partition gridmap
2) 

Date: 29/8/2016
/*************************************************************/
class SP_Comp : public Renderer
{
	IntersectedPoints intersected[4];

public:

	/******************** constructor/destructor **********************/
	SP_Comp();
	SP_Comp(const SP_Comp& copyMe);
	~SP_Comp();

	/************************** Core functions ****************************/
	void Init(const char* name, Vector3 pos, Vector3 scale);

	/************************** Get/set functions ****************************/

	/************************** Update grid function ****************************/
	//total SP grids
	void UpdateSP(int totalGrids_X, int totalGrids_Y);
};

#endif