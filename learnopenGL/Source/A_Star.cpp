#include "AI_Map.h"
#include "CoreUtilities.h"

/********************************************************************************
A* Node
********************************************************************************/
AS_Node::AS_Node()
{
	F = G = H = 0;
	dirX = dirY = 0;
}

AS_Node::~AS_Node(){}

/********************************************************************************
Set node as central node
********************************************************************************/
void AS_Node::Init(int x, int y, bool is_wall)
{
	if (is_wall)
		F = G = H = -1;
	this->x = x;
	this->y = y;
}

void AS_Node::Set_G(unsigned cost)
{
	G = cost;
	F = G + H;
}

void AS_Node::Set_H(unsigned cost)
{
	G = cost;
	F = G + H;
}

void AS_Node::SetGH(unsigned G, unsigned H)
{
	this->G = G;
	this->H = H;
	F = G + H;
}

/********************************************************************************
set as starting node
********************************************************************************/
void AS_Node::Set_asStart(AS_Node& endNode)
{
	Set_AsCentral(endNode.x, endNode.y);

	//calculate F cost------------------------------------//
	int xDist = abs(dirX);
	int yDist = abs(dirY);
	int longestDist = (xDist > yDist) ? xDist : yDist;
	int shortestDist = (xDist <= yDist) ? xDist : yDist;

	int straight_hCost = (longestDist - shortestDist) * 10;
	int diagonal_hCost = shortestDist * 14;

	SetGH(0, straight_hCost + diagonal_hCost);
}

/********************************************************************************
set as central point
********************************************************************************/
void AS_Node::Set_AsCentral(int endX, int endY)
{
	dirX = endX - x;
	dirY = endY - y;
}

/********************************************************************************
calculate surrounding nodes if this is the center
********************************************************************************/
void AS_Node::CalculateSurroundings(AS_Node nodeList[])
{
	//Nodes 1, 3, 5, 7 cost: 10--------------------//
	nodeList[1].Set_G(G + 10);
	nodeList[3].Set_G(G + 10);
	nodeList[5].Set_G(G + 10);
	nodeList[7].Set_G(G + 10);

	//Nodes 0, 2, 6, 8 cost: 10--------------------//
	nodeList[0].Set_G(G + 14);
	nodeList[2].Set_G(G + 14);
	nodeList[6].Set_G(G + 14);
	nodeList[8].Set_G(G + 14);
}

/********************************************************************************
Resets non-wall node
********************************************************************************/
void AS_Node::Reset()
{
	if (F != -1)
		F = G = H = 0;
}

bool AS_Node::isWall(){ return F == -1; }

/********************************************************************************
A* Node
********************************************************************************/
void AI_Map::Init_AStar()
{
	//create nodes to cover entire map------------------------------------------------------//
	nodeList = new AS_Node*[total_tiles_X];
	bool isWall = false;

	for (int x = 0; x < total_tiles_X; ++x)
	{
		nodeList[x] = new AS_Node[total_tiles_Y];

		for (int y = 0; y < total_tiles_Y; ++y)
		{
			isWall = false;

			//if is a wall, not passable----------------//
			if (pathMap[x][y] == -1)
				isWall = true;

			nodeList[x][y].Init(x, y, isWall);
		}
	}

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

/********************************************************************************
Start A* path finding
********************************************************************************/
void AI_Map::Start_AStar(int startX, int startY, int endX, int endY)
{
	StartPathFind(startX, startY, endX, endY);
}

void AI_Map::Clear_AStar()
{
	if (AS_start->isActive())
	{
		AS_start->SetActive(false);
		AS_end->SetActive(false);
	}
}