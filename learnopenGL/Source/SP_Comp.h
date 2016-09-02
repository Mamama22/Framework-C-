#ifndef SP_COMP_H
#define SP_COMP_H
#include "Renderer.h"

/*************************************************************
/*************************************************************/
struct IntersectedPoints
{
	int x;
	int y;

	IntersectedPoints(){ SetNone(); }
	~IntersectedPoints(){}

	void SetNone(){ x = y = -1; }
	void SetPoint(int x, int y){ this->x = x; this->y = y; }

	IntersectedPoints& operator= (IntersectedPoints& copyMe)
	{
		x = copyMe.x;
		y = copyMe.y;
		return *this;
	}

	bool operator!= (IntersectedPoints& checkMe)
	{
		return x != checkMe.x || y != checkMe.y;
	}
};

/*************************************************************
Author: Tan Yie Cher

Spartial partition component, makes sure each Entity only have ONE
Spartial partition will only take the first SP comp found on each Entity
TO be used with GridMap Entity

How it works:
1) Assumes parent Entity of this comp. will be added as child of the Spartial Partition gridmap
2) Gridmap will get the updated intersected points from SP comp if it's entity transforms

Date: 29/8/2016
/*************************************************************/
class SP_Comp : public Render_InWorld
{
public:

	enum INTERSECT_TYPE
	{
		ONE,
		TWO,
		THREE,
		FOUR,
		OUT_OF_BOUNDS,
	};

private:

	IntersectedPoints intersected[4];	//store intersected points, follow standard order(fill index 0 - 3)
	bool stage1_transformed;	//theres a transformation at stage 1
	INTERSECT_TYPE intersect_type;

public:

	/******************** constructor/destructor **********************/
	SP_Comp();
	SP_Comp(const SP_Comp& copyMe);
	~SP_Comp();

	/************************** Core functions ****************************/
	void Init(const char* name, Vector3 pos, Vector3 scale);

	void PreUpdate();

	/******************** Entity Transformation function: CALLED BY ENTITY ONLY **********************/
	void CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform);

	/************************** Get/set functions ****************************/
	bool is_stage1_transformed();
	INTERSECT_TYPE Get_IntersectType();

	/************************** Update grid function ****************************/
	//total SP grids
	//returns: type of intersection
	void UpdateSP(int totalGrids_X, int totalGrids_Y, const Vector3& gridmap_pos, const Vector3& sp_grid_scale,
		IntersectedPoints prev_storePoints[], IntersectedPoints current_storePoints[]);
};

#endif