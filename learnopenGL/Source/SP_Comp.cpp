#include "SP_Comp.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
SP_Comp::SP_Comp() : Render_InWorld()
{
}

SP_Comp::SP_Comp(const SP_Comp& copyMe) : Render_InWorld(copyMe)
{
}

SP_Comp::~SP_Comp()
{
}


/********************************************************************************
Init
********************************************************************************/
void SP_Comp::Init(const char* name, Vector3 pos, Vector3 scale)
{
	Renderer::Init(name, CU::shared.debug_quad, pos, scale);
}

void SP_Comp::PreUpdate()
{
	Component::PreUpdate();
	stage1_transformed = false;
}

/********************************************************************************
Update spartial partition
param total_intersect: total intersected grids
param storePoints: storePoints will copy values of this SP comp's intersected
********************************************************************************/
Vector3 corners[4];
int xGrid[4];
int yGrid[4];
float xVal, yVal;
void SP_Comp::UpdateSP(int totalGrids_X, int totalGrids_Y, const Vector3& gridmap_pos, const Vector3& sp_grid_scale,
	IntersectedPoints prev_storePoints[], IntersectedPoints current_storePoints[])
{
	//set prev points-----------------------------------------------------------//
	for (int i = 0; i < 4; ++i)
	{
		prev_storePoints[i] = intersected[i];
		current_storePoints[i].SetNone();
		intersected[i].SetNone();	//reset intersected
	}

	//get local pos, entity.pos - gridmap.pos--------------------------------------------//
	Vector3 localPos = transform.pos - gridmap_pos;

	//get the 4 corners: LL -> UL -> UU -> LU (same as quad vertex arrangement)--------------------------------------------//
	Vector3 halfScale = transform.scale * 0.5f;

	//LL
	corners[0] = localPos - halfScale;

	//UL
	corners[1] = localPos - halfScale;
	corners[1].x += transform.scale.x;

	//UU
	corners[2] = localPos + halfScale;

	//LU
	corners[3] = localPos + halfScale;
	corners[3].x -= transform.scale.x;

	//Get the intersected grids of each corner---------------------------------------------------------------------//
	for (int i = 0; i < 4; ++i)
	{
		xVal = corners[i].x * (1.f / sp_grid_scale.x);
		yVal = corners[i].y * (1.f / sp_grid_scale.y);


		xGrid[i] = (int)(xVal >= 0.f) ? xVal : -1;
		yGrid[i] = (int)(yVal >= 0.f) ? yVal : -1;
	}
	cout << "Y grid: " << int(-0.1f) << endl;

	//keep them in bounds------------------------------------------------------------------//
	if (xGrid[0] < 0)
	{
		if (xGrid[1] >= 0)
			xGrid[0] = xGrid[3] = 0;
		else
		{
			intersect_type = OUT_OF_BOUNDS;
			return;
		}
	}
	else if (xGrid[2] >= totalGrids_X)
	{
		if (xGrid[0] < totalGrids_X)
			xGrid[1] = xGrid[2] = totalGrids_X - 1;
		else
		{
			intersect_type = OUT_OF_BOUNDS;
			return;
		}
	}
	if (yGrid[0] < 0)
	{
		if (yGrid[2] >= 0)
			yGrid[0] = yGrid[1] = 0;
		else
		{
			intersect_type = OUT_OF_BOUNDS;
			return;
		}
	}
	else if (yGrid[2] >= totalGrids_Y)
	{
		if (yGrid[0] < totalGrids_Y)
			yGrid[3] = yGrid[2] = totalGrids_Y - 1;
		else
		{
			intersect_type = OUT_OF_BOUNDS;
			return;
		}
	}

	//check the grid/s intersected
	//1 grid only----------------------------------------------------------//
	if (xGrid[0] == xGrid[2] && yGrid[0] == yGrid[2])
	{
		intersected[0].SetPoint(xGrid[0], yGrid[0]);
		current_storePoints[0] = intersected[0];
		intersect_type = ONE;
	}

	//2 grids: XX----------------------------------------------------//
	else if (xGrid[0] != xGrid[2] && yGrid[0] == yGrid[2])
	{
		intersected[0].SetPoint(xGrid[0], yGrid[0]);
		intersected[1].SetPoint(xGrid[1], yGrid[1]);
		current_storePoints[0] = intersected[0];
		current_storePoints[1] = intersected[1];
		intersect_type = TWO;
	}

	//2 grids: YY----------------------------------------------------//
	else if (xGrid[0] == xGrid[2] && yGrid[0] != yGrid[2])
	{
		intersected[0].SetPoint(xGrid[0], yGrid[0]);
		intersected[1].SetPoint(xGrid[2], yGrid[2]);
		current_storePoints[0] = intersected[0];
		current_storePoints[1] = intersected[1];
		intersect_type = THREE;
	}

	//4 Grids------------------------------------------------------//
	else
	{
		intersected[0].SetPoint(xGrid[0], yGrid[0]);
		intersected[1].SetPoint(xGrid[1], yGrid[1]);
		intersected[2].SetPoint(xGrid[2], yGrid[2]);
		intersected[3].SetPoint(xGrid[3], yGrid[3]);
		current_storePoints[0] = intersected[0];
		current_storePoints[1] = intersected[1];
		current_storePoints[2] = intersected[2];
		current_storePoints[3] = intersected[3];
		intersect_type = FOUR;
	}
}

/********************************************************************************
Calculate TRS with parents
********************************************************************************/
void SP_Comp::CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform)
{
	stage1_transformed = transform.isTransforming();

	Component::CalculateTRS_WithParent(parentRotMat, GrandParentTransform);
	transform.Calculate_TRS_withParent(parentRotMat);
}

bool SP_Comp::is_stage1_transformed(){ return stage1_transformed; }
SP_Comp::INTERSECT_TYPE SP_Comp::Get_IntersectType(){ return intersect_type; }