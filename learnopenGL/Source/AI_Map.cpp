#include "AI_Map.h"
#include "CoreUtilities.h"

AI_Map::AI_Map()
{
	AS_start = AS_end = NULL;
	pathFind_step = -1;
	state = IDLE;
}

AI_Map::~AI_Map()
{
	for (int i = 0; i < total_tiles_X; ++i)
		delete[] pathMap[i];
	delete[] pathMap;

	for (int i = 0; i < total_tiles_X; ++i)
		delete[] nodeList[i];
	delete[] nodeList;
}

/********************************************************************************
Init AI map
********************************************************************************/
void AI_Map::Init(Vector3 pos, vector< vector<int> >& pathMap_passIn, float tileScale, int total_tiles_X, int total_tiles_Y)
{
	Entity::Init(pos, Vector3(1, 1, 1));

	this->tileScale = tileScale;
	total_passable_tiles = 0;

	//scene map----------------------------------------------------------------------------------------//
	pathMap = new int*[total_tiles_X];	//wtf, pathMap_template cannot directly new bool*[]
	
	for (int i = 0; i < total_tiles_X; ++i)
	{
		pathMap[i] = new int[total_tiles_Y];
		for (int g = 0; g < total_tiles_Y; ++g)
			pathMap[i][g] = pathMap_passIn[i][g];
	}

	//Add renderer-------------------------------------------------------------------------------------//
	pathMap_renderer = new Render_GridMap;
	//intersectedGrid->Init("Asd", CU::shared.quad_start0, Vector3(0, 0, 0), Vector3(mapScaleX / (float)total_SP_X, mapScaleY / (float)total_SP_Y, 1));
	pathMap_renderer->Init("d", TILEMAP_AI, pos, tileScale, total_tiles_X, total_tiles_Y);

	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < total_tiles_Y; ++y)
		{
			if (pathMap[x][y] >= 0)	//valid tile
			{
				total_passable_tiles++;
				pathMap_renderer->SetTile(x, y, 1);
			}
			else	//wall
			{
				pathMap_renderer->SetTileEmpty(x, y);
				pathMap[x][y] = -1;	//is a wall
			}
		}
	}

	pathMap_renderer->RecalculateMesh();
	AddComponent(pathMap_renderer);

	this->total_tiles_X = total_tiles_X;
	this->total_tiles_Y = total_tiles_Y;

	//a star-------------------------------------------------------------------------------------//
	Init_AStar();
	Init_BFS();
}

void AI_Map::Update_Stage1()
{
}

void AI_Map::Update_Steps()
{
	switch (state)
	{
	case IDLE:
		break;
	case SHOW_BFS:
		onClick_BFS();
		break;
	case SHOW_A_STAR:
		break;
	}
}

/********************************************************************************
Add path to be displayed visually
********************************************************************************/
void AI_Map::AddPath_ToDisplay(AI_Comp* AI_comp)
{
	//Set the paths------------------------------------------------------//
	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < total_tiles_Y; ++y)
			pathMap_renderer->SetTile(x, y, 1);
	}

	//Set path points to diff. color---------------------------------------//
	for (int i = AI_comp->Get_target_index(); i < AI_comp->Get_currentPathSize(); ++i)	//xPoints and yPoints same size
	{
		pathMap_renderer->SetTile(AI_comp->Get_xPoint(i), AI_comp->Get_yPoint(i), 3);
	}

	pathMap_renderer->RecalculateMesh();
}

/********************************************************************************
Plot a path, called by AI_Comp
param pathMap: path map that will be modified to contain the plotted path
********************************************************************************/
bool AI_Map::findPath(AI_Comp* AI_comp, const int startPt_X, const int startPt_Y, const int endPt_X, const int endPt_Y)
{
	//if have prev. path, remove-------------------------------------------//
	AI_comp->RemovePath();

	//out of bounds check--------------------------------------------------//
	if (startPt_X < 0 || startPt_Y < 0 || endPt_X >= total_tiles_X || endPt_Y >= total_tiles_Y)
		return false;
	
	//keep track of point----------------------------------------------//
	int point_index = 0;

	//find path btw the 2 points and overwrite pathMap--------------------//
	int midPoint_Y = (startPt_Y > endPt_Y) ? startPt_Y : endPt_Y;	//highest point's Y
	int midPoint_X = (startPt_X > endPt_X) ? endPt_X : startPt_X;	//lowest point's X

	int x_pathTile = startPt_X;
	int y_pathTile = startPt_Y;

	//direction of midpoint from start point-----------------------------//
	int dirX = (startPt_X > midPoint_X) ? -1 : 1;
	int dirY = (startPt_Y > midPoint_Y) ? -1 : 1;

	if (midPoint_X == startPt_X)
		dirX = 0;
	if (midPoint_Y == startPt_Y)
		dirY = 0;

	do
	{
		AI_comp->AddPoint(x_pathTile, y_pathTile, point_index);

		if (x_pathTile != midPoint_X)
			x_pathTile += dirX;
		if (y_pathTile != midPoint_Y)
			y_pathTile += dirY;
		point_index++;

	} while (!(x_pathTile == midPoint_X && y_pathTile == midPoint_Y));
	
	//once reached midpoint, go to end point---------------------------//
	dirX = (endPt_X > midPoint_X) ? 1 : -1;
	dirY = (endPt_Y > midPoint_Y) ? 1 : -1;

	if (midPoint_X == endPt_X)
		dirX = 0;
	if (midPoint_Y == endPt_Y)
		dirY = 0;

	do
	{
		AI_comp->AddPoint(x_pathTile, y_pathTile, point_index);
		
		if (x_pathTile != endPt_X)
			x_pathTile += dirX;
		if (y_pathTile != endPt_Y)
			y_pathTile += dirY;
		point_index++;

	} while (!(x_pathTile == endPt_X && y_pathTile == endPt_Y));

	return true;
}

/********************************************************************************
Get point pos (transform.pos is at bottom left corner)
********************************************************************************/
Vector3 AI_Map::Get_PointPos(int x, int y)
{
	return Vector3(transform.pos.x + (tileScale * x), transform.pos.y + (tileScale * y), 0);
}

/********************************************************************************
Setup for path-find
********************************************************************************/
void AI_Map::StartPathFind(int startX, int startY, int endX, int endY)
{
	pathFind_step = 0;
	this->pathFind_startX = startX;
	this->pathFind_startY = startY;
	this->pathFind_endX = endX;
	this->pathFind_endY = endY;

	//renderers for start end pos-----------------------------------------------//
	AS_start->Relocate(Get_PointPos(startX, startY));
	AS_end->Relocate(Get_PointPos(endX, endY));
	AS_start->SetActive(true);
	AS_end->SetActive(true);
}

/********************************************************************************
Get/Set
********************************************************************************/
int AI_Map::getTotal_tilesX(){ return total_tiles_X; }
int AI_Map::getTotal_tilesY(){ return total_tiles_Y; }
float AI_Map::Get_tileScale(){ return tileScale; }