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

	for (int x = 0; x < total_tiles_X; ++x)
		delete[] bfs_GridList[x];
	delete[] bfs_GridList;
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

	//BFS----------------------------------------------------------------------------------//
	Init_BFS();

	//set start and end point renderer------------------------------------------------------------------//
	AS_start = new Render_InWorld;
	AS_start->Init("fuck u", CU::shared.quad_start0_1, Vector3(0, 0, 0), Vector3(tileScale, tileScale, 1.f));

	AS_end = new Render_InWorld;
	AS_end->Init("fuck u", CU::shared.quad_start0_1, Vector3(0, 0, 0), Vector3(tileScale, tileScale, 1.f));

	AddComponent(AS_start);
	AddComponent(AS_end);

	AS_start->SetAlpha(0.7f);
	AS_end->SetAlpha(0.7f);

	AS_start->SetActive(false);
	AS_end->SetActive(false);
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
	//Reset paths------------------------------------------------------//
	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < total_tiles_Y; ++y)
		{
			if (pathMap[x][y] >= 0)	//valid tile
				pathMap_renderer->SetTile(x, y, 1);
			else	//wall
				pathMap_renderer->SetTileEmpty(x, y);		
		}
	}

	//Set path points to diff. color (from end to start)---------------------------------------//
	for (int i = 0; i <= AI_comp->Get_target_index(); ++i)	//xPoints and yPoints same size
	{
		pathMap_renderer->SetTile(AI_comp->Get_xPoint(i), AI_comp->Get_yPoint(i), 3);
	}

	pathMap_renderer->RecalculateMesh();
}

/********************************************************************************
Plot a path, called by AI_Comp
param pathMap: path map that will be modified to contain the plotted path
return: -1 if no path, size of path if there's one
********************************************************************************/
int AI_Map::findPath(AI_Comp* AI_comp, const int startPt_X, const int startPt_Y, const int endPt_X, const int endPt_Y)
{
	//if have prev. path, remove-------------------------------------------//
	AI_comp->RemovePath();

	//out of bounds check--------------------------------------------------//
	if (startPt_X < 0 || startPt_Y < 0 || endPt_X >= total_tiles_X || endPt_Y >= total_tiles_Y)
		return -1;
	
	//Find path with BFS------------------------------------------------------//
	Start_BFS(startPt_X, startPt_Y, endPt_X, endPt_Y);

	//find path-------------------------------------------//
	while (!Update_BFS()){}

	//get path-----------------------------------------//
	AI_comp->RemovePath();	//remove prev. path

	int size = GetOptimal_path_BFS(AI_comp->Get_XY_Grids());

	//simulate 100 zombies----------------------------//
	GetOptimal_path_BFS(AI_comp->Get_XY_Grids());

	AI_comp->Set_PathSize(size);

	return size;
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
clear paths
********************************************************************************/
void AI_Map::Clear_Display()
{
	//Reset paths------------------------------------------------------//
	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < total_tiles_Y; ++y)
		{
			if (pathMap[x][y] >= 0)	//valid tile
				pathMap_renderer->SetTile(x, y, 1);
			else	//wall
				pathMap_renderer->SetTileEmpty(x, y);
		}
	}

	pathMap_renderer->RecalculateMesh();
}

/********************************************************************************
Get/Set
********************************************************************************/
int AI_Map::getTotal_tilesX(){ return total_tiles_X; }
int AI_Map::getTotal_tilesY(){ return total_tiles_Y; }
float AI_Map::Get_tileScale(){ return tileScale; }