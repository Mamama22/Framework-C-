#include "AI_Comp.h"
#include "CoreUtilities.h"
#include "AI_Map.h"

AI_Comp::AI_Comp()
{ 
	AI_Map_id = -1; 

	//assumes max path is 200 tiles----------------------//
	xPoints.resize(200);
	yPoints.resize(200);
	pathSize = 0;
	target_index = -1;
	state = IDLE;
}


AI_Comp::AI_Comp(const AI_Comp& copyMe)
{ 
	AI_Map_id = copyMe.AI_Map_id; 
}

AI_Comp::~AI_Comp()
{
}


/********************************************************************************
Init
********************************************************************************/
void AI_Comp::Init(const char* name, int AI_Map_id, bool display_path)
{
	Component::Init(name, Vector3(0, 0, 0), Vector3(1, 1, 1));

	//get id and map size of AI_Map------------------------------------//
	this->AI_Map_id = AI_Map_id;

	if (CU::entityMan.CheckEntityType<AI_Map>(AI_Map_id))
	{
		AI_Map* gg = static_cast<AI_Map*>(CU::entityMan.GetEntity(AI_Map_id));	//get the AI_Map entity

		//width and height----------------------------------------------//	
		totalTiles_X = gg->getTotal_tilesX();
		totalTiles_Y = gg->getTotal_tilesY();
		tileScale = gg->Get_tileScale();
	}
	else
		AI_Map_id = -1;

	this->display_path = display_path;
}

/********************************************************************************
Find a path at the AI_Map
********************************************************************************/
AI_Map* gg;
bool AI_Comp::findPath(int startPt_X, int startPt_Y, int endPt_X, int endPt_Y)
{
	if (AI_Map_id == -1)
		return false;
	
	gg = static_cast<AI_Map*>(CU::entityMan.GetEntity(AI_Map_id));

	if (gg->findPath(this, startPt_X, startPt_Y, endPt_X, endPt_Y))
	{
		state = PATH_FOLLOWING;

		//Set start point as target----------------------------------//
		target_index = 0;

		if (display_path)
			gg->AddPath_ToDisplay(this);

		return true;
	}
	return false;
}


/********************************************************************************
Get target point pos, if reached, AI Comp will automatically assign a new one
********************************************************************************/
Vector3 AI_Comp::GetTargetPointPos()
{
	gg = static_cast<AI_Map*>(CU::entityMan.GetEntity(AI_Map_id));
	return gg->Get_PointPos(xPoints[target_index], yPoints[target_index]);
}

/********************************************************************************
Get next points pos (Assume entity reached target point)
********************************************************************************/
void AI_Comp::GetNextPoint()
{
	//go next index--------------------------------//
	if (target_index < pathSize - 1)	//target_index == pathSize - 1: reached dest. point
		target_index++;

	if (display_path)
	{
		gg = static_cast<AI_Map*>(CU::entityMan.GetEntity(AI_Map_id));
		gg->AddPath_ToDisplay(this);
	}
}

/********************************************************************************
Remove path
can be called by AI_Map too
********************************************************************************/
void AI_Comp::RemovePath()
{
	target_index = -1;
	pathSize = 0;
}

/********************************************************************************
For AI_Map, set points
********************************************************************************/
void AI_Comp::AddPoint(int x, int y, int index)
{
	xPoints[index] = x;
	yPoints[index] = y;
	pathSize++;
}

/********************************************************************************
Reached end point
********************************************************************************/
bool AI_Comp::Reached_Dest()
{
	return target_index == pathSize - 1;
}

int AI_Comp::Get_currentPathSize(){ return pathSize; }
int AI_Comp::Get_target_index(){ return target_index; }
int AI_Comp::Get_xPoint(int index){ return xPoints[index]; }
int AI_Comp::Get_yPoint(int index){ return yPoints[index]; }
float AI_Comp::Get_tileScale(){ return tileScale; }

void AI_Comp::Exit()
{
}