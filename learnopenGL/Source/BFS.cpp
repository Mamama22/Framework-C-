#include "AI_Map.h"
#include "CoreUtilities.h"

/********************************************************************************
BFS grid
********************************************************************************/
BFS_Grid::BFS_Grid()
{
	visited = false;
}

BFS_Grid::~BFS_Grid(){}

/********************************************************************************
Set the fixed X, Y pos
********************************************************************************/
void BFS_Grid::Init(int x, int y, bool isWall)
{
	pos.x = x;
	pos.y = y;
	this->isWall = isWall;
}

/********************************************************************************
Reset for new calculation
********************************************************************************/
void BFS_Grid::Reset()
{
	visited = false;
	came_from.x = came_from.y = -1;
}

/********************************************************************************
Set tile that this came from
********************************************************************************/
void BFS_Grid::Set_CameFrom(XY_grid came_pos)
{
	came_from.x = came_pos.x;
	came_from.y = came_pos.y;
}

void BFS_Grid::SetAs_Visited()
{
	visited = true;
}


/********************************************************************************
init BFS
********************************************************************************/
void AI_Map::Init_BFS()
{
	//create nodes to cover entire map------------------------------------------------------//
	bfs_GridList = new BFS_Grid*[total_tiles_X];
	bool isWall = false;
	frontier_lastIndex = 0;

	for (int x = 0; x < total_tiles_X; ++x)
	{
		bfs_GridList[x] = new BFS_Grid[total_tiles_Y];

		for (int y = 0; y < total_tiles_Y; ++y)
		{
			isWall = false;

			//if is a wall, not passable----------------//
			if (pathMap[x][y] == -1)
				isWall = true;

			bfs_GridList[x][y].Init(x, y, isWall);
		}
	}

	//Set frontier to fixed size, so no need to keep creating and popping mem. space----------//
	frontier.resize(total_tiles_X * total_tiles_Y);
}

/*******************************************************************************
Start BFS
********************************************************************************/
void AI_Map::Start_BFS(int startX, int startY, int endX, int endY)
{
	state = SHOW_BFS;
	StartPathFind(startX, startY, endX, endY);

	//reset grids-----------------------------------------------------//
	for (int x = 0; x < total_tiles_X; ++x)
	{
		for (int y = 0; y < total_tiles_Y; ++y)
			bfs_GridList[x][y].Reset();
	}

	frontier[pathFind_step] = &bfs_GridList[startX][startY];
	frontier[pathFind_step]->SetAs_Visited();	///mark as visited
	frontier_lastIndex = 0;	//last index is 0
}

/*******************************************************************************
Display optimal path on mesh
********************************************************************************/
XY_grid thePos;
void AI_Map::Display_OptimalPath_BFS()
{
	XY_grid currentIndex;
	currentIndex.x = pathFind_endX;
	currentIndex.y = pathFind_endY;

	bool breakOut = false;
	do
	{
		//check if reached starting grid----------------------------------------------//
		if (currentIndex.x == pathFind_startX && currentIndex.y == pathFind_startY)
			breakOut = true;

		//get pos of grid along optimal path------------------------------------------//
		pathMap_renderer->SetTile(currentIndex.x, currentIndex.y, 3);	//mark as optimal (No need use pos, same as 2D index)

		//get came from---------------------------------------------------------------//
		currentIndex = bfs_GridList[currentIndex.x][currentIndex.y].came_from;

	} while (!breakOut);

	pathMap_renderer->RecalculateMesh();	//update mesh
}

/*******************************************************************************
Start BFS: loop that loops when player clicks
if start grid is in a wall, pathFind_step >= frontier's size in next iteration and will quit
********************************************************************************/
void AI_Map::onClick_BFS()
{
	//if not out of loop----------------------------------------------//
	if (pathFind_step >= frontier_lastIndex + 1)
	{
		//if reached end, show the optimal path-------------------------------//
		Display_OptimalPath_BFS();
		state = IDLE;
		return;
	}

	//Get pos-----------------------------//
	thePos.x = frontier[pathFind_step]->pos.x;
	thePos.y = frontier[pathFind_step]->pos.y;
	pathMap_renderer->SetTile(thePos.x, thePos.y, 2);	//mark as visited

	//Get ajacent grids-------------------------------------//
	if (thePos.y < total_tiles_Y - 1)
	{
		Add_AjacentTo_Frontier(&bfs_GridList[thePos.x][thePos.y + 1]);
	}
	if (thePos.x > 0)
	{
		Add_AjacentTo_Frontier(&bfs_GridList[thePos.x - 1][thePos.y]);
	}
	if (thePos.y > 0)
	{
		Add_AjacentTo_Frontier(&bfs_GridList[thePos.x][thePos.y - 1]);
	}
	if (thePos.x < total_tiles_X - 1)
	{
		Add_AjacentTo_Frontier(&bfs_GridList[thePos.x + 1][thePos.y]);
	}

	//update data---------------------------------------------------------//
	pathFind_step++;
	pathMap_renderer->RecalculateMesh();	//update mesh
}

/********************************************************************************
Ajacent BFS grid added to frontier
param count: how much ahead of pathFind_step to add checked grid
********************************************************************************/
bool AI_Map::Add_AjacentTo_Frontier(BFS_Grid* addMe)
{
	//make sure is non of these------------------------------//
	if (addMe->isWall || addMe->visited)
		return false;

	frontier_lastIndex++;	//last index now increased by 1

	//Add tiles to frontier--------------------------------------------------------------------------//
	frontier[frontier_lastIndex] = addMe;
	frontier[frontier_lastIndex]->SetAs_Visited();	//set as visited
	frontier[frontier_lastIndex]->Set_CameFrom(thePos);	//set came from

	pathMap_renderer->SetTile(addMe->pos.x, addMe->pos.y, 0);	//mark as waiting
	
	return true;
}

/********************************************************************************
clear BFS
********************************************************************************/
void AI_Map::Clear_BFS()
{

}