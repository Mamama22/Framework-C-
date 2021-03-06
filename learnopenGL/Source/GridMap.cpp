#include "GridMap.h"
#include "CoreUtilities.h"

SP_Grid::SP_Grid()
{
	entityCount = 0;

	for (int i = 0; i < MAX_ENTITY_PER_GRID; ++i)
	{
		entityIntersected[i] = -1;
	}
}

SP_Grid::SP_Grid(const SP_Grid& sd)
{
	entityCount = sd.entityCount;

	for (int i = 0; i < MAX_ENTITY_PER_GRID; ++i)
	{
		entityIntersected[i] = sd.entityIntersected[i];
	}
}

SP_Grid::~SP_Grid(){}

/********************************************************************************
Add entity
returns: index of entity in this gridmap
********************************************************************************/
void SP_Grid::AddEntity(int handle)
{
	entityIntersected[entityCount++] = handle;
}

/********************************************************************************
Remove entity
********************************************************************************/
void SP_Grid::RemoveEntity(int handle)
{
	for (int i = 0; i < entityCount; ++i)
	{
		if (entityIntersected[i] == handle)
		{
			entityIntersected[i] = entityIntersected[entityCount - 1];
			entityIntersected[entityCount - 1] = -1;
			entityCount--;
			break;
		}
	}
}

void SP_Grid::ClearAll()
{
	entityCount = 0;
}

/********************************************************************************
Get set
********************************************************************************/
int* SP_Grid::GetEntityList(){return entityIntersected;}
int SP_Grid::Get_entityCount(){return entityCount;}

GridMap::GridMap(){}
GridMap::~GridMap(){}

/********************************************************************************
init
********************************************************************************/
void GridMap::Init(Vector3 pos, TILEMAP_ENUM tilemesh, float tileScale, int totalX_tiles, int totalY_tiles, int total_SP_X, int total_SP_Y)
{
	float mapScaleX = tileScale * totalX_tiles;
	float mapScaleY = tileScale * totalY_tiles;
	Entity::Init(pos, Vector3(1.f, 1.f, 1.f));

	//total tiles combined for entire map---------------------------------//
	this->totalX_tiles = totalX_tiles;
	this->totalY_tiles = totalY_tiles;

	this->tileScale = tileScale;

	//Spartial partition-------------------------------------------//
	this->total_SP_X = total_SP_X;
	this->total_SP_Y = total_SP_Y;
	added_debug_SP = false;	//added last
}

/********************************************************************************
Add debug grids to renderer
********************************************************************************/
void GridMap::Add_DebugSP_renderers()
{
	float mapScaleX = tileScale * totalX_tiles;
	float mapScaleY = tileScale * totalY_tiles;
	float gridScaleX = mapScaleX / (float)total_SP_X;
	float gridScaleY = mapScaleY / (float)total_SP_Y;

	//X sp lines-------------------------------------------------------------//
	Render_Repetitive* render_line = new Render_Repetitive();
	render_line->Init("SD", CU::shared.quad_start0, transform.GetPos(), Vector3(mapScaleX, 2, 1), Vector3(0, mapScaleY / (float)total_SP_Y, 0), total_SP_X + 1);
	AddComponent(render_line);

	//Y sp lines-------------------------------------------------------------//
	render_line = new Render_Repetitive();
	render_line->Init("SD", CU::shared.quad_start0, transform.GetPos(), Vector3(2, mapScaleY, 1), Vector3(mapScaleX / (float)total_SP_X, 0, 0), total_SP_Y + 1);
	AddComponent(render_line);

	//highlight intersected-----------------------------------------------//
	intersectedGrid = new Render_GridMap;
	//intersectedGrid->Init("Asd", CU::shared.quad_start0, Vector3(0, 0, 0), Vector3(mapScaleX / (float)total_SP_X, mapScaleY / (float)total_SP_Y, 1));
	intersectedGrid->Init("d", TILEMAP_PERRY, transform.GetPos(), gridScaleX, total_SP_X, total_SP_Y);
	intersectedGrid->SetAlpha(0.5f);
	intersectedGrid->SetAllTilesEmpty();
	intersectedGrid->RecalculateMesh();
	AddComponent(intersectedGrid);
	intersectedGrid->SetActive(false);

	sp_GridScale.Set(gridScaleX, gridScaleY, 1.f);

	//SP grids----------------------------------------------------------------//
	SP_Grids.resize(total_SP_X * total_SP_Y);
}

void GridMap::PreUpdate()
{
	//make sure debug SP added last (renders on top)
	if (!added_debug_SP)
	{
		added_debug_SP = true;
		Add_DebugSP_renderers();
	}
	
	for (int i = 0; i < SP_Grids.size(); ++i)
		SP_Grids[i].ClearAll();
}

/********************************************************************************
Update stage 2: the pos before update
********************************************************************************/
void GridMap::Update_Stage2()
{
	bool changes = false;	//changes in SP

	//calculate pos and TRS-----------------------------------------------------//
	//parent(this) and all children(and their SP comp) will have updated pos and TRS
	Entity::Update_Stage2();

	//Check spartial partition with updated pos--------------------------------//
	for (int i = 0; i < spComp_List.size(); ++i)
	{
		if (!spComp_List[i]->isActive())
			continue;

		spComp_List[i]->UpdateSP(total_SP_X, total_SP_Y, transform.pos, sp_GridScale, prev_storePoints, current_storePoints);
		intersect_type = spComp_List[i]->Get_IntersectType();

		if (intersect_type != SP_Comp::OUT_OF_BOUNDS)
		{
			AddSP_ToGrids(spComp_List[i], current_storePoints);
		}
	}

	//Update gridmap debug--------------------------------------------------------------------//
	for (int x = 0; x < total_SP_X; ++x)
	{
		for (int y = 0; y < total_SP_Y; ++y)
		{
			if (SP_Grids[(x * total_SP_Y) + y].Get_entityCount() == 0)
				intersectedGrid->SetTileEmpty(x, y);
			else
				intersectedGrid->SetTile(x, y, 1);
		}
		intersectedGrid->RecalculateMesh();
	}
}

void GridMap::RemoveSP_FromAllGrids(SP_Comp* removeMe, IntersectedPoints prev_points[])
{
	if (prev_intersect_type == SP_Comp::ONE)
	{
		SP_Grids[(prev_points[0].x * total_SP_Y) + prev_points[0].y].RemoveEntity(removeMe->GetParentHandle());
	}
	else if (prev_intersect_type == SP_Comp::TWO || prev_intersect_type == SP_Comp::THREE)
	{
		SP_Grids[(prev_points[0].x * total_SP_Y) + prev_points[0].y].RemoveEntity(removeMe->GetParentHandle());
		SP_Grids[(prev_points[1].x * total_SP_Y) + prev_points[1].y].RemoveEntity(removeMe->GetParentHandle());
	}
	else if (prev_intersect_type == SP_Comp::FOUR)
	{
		SP_Grids[(prev_points[0].x * total_SP_Y) + prev_points[0].y].RemoveEntity(removeMe->GetParentHandle());
		SP_Grids[(prev_points[1].x * total_SP_Y) + prev_points[1].y].RemoveEntity(removeMe->GetParentHandle());
		SP_Grids[(prev_points[2].x * total_SP_Y) + prev_points[2].y].RemoveEntity(removeMe->GetParentHandle());
		SP_Grids[(prev_points[3].x * total_SP_Y) + prev_points[3].y].RemoveEntity(removeMe->GetParentHandle());
	}
}

void GridMap::AddSP_ToGrids(SP_Comp* addMe, IntersectedPoints current_points[])
{
	if (intersect_type == SP_Comp::ONE)
	{
		SP_Grids[(current_points[0].x * total_SP_Y) + current_points[0].y].AddEntity(addMe->GetParentHandle());
	}
	else if (intersect_type == SP_Comp::TWO || intersect_type == SP_Comp::THREE)
	{
		SP_Grids[(current_points[0].x * total_SP_Y) + current_points[0].y].AddEntity(addMe->GetParentHandle());
		SP_Grids[(current_points[1].x * total_SP_Y) + current_points[1].y].AddEntity(addMe->GetParentHandle());
	}
	else if (intersect_type == SP_Comp::FOUR)
	{
		SP_Grids[(current_points[0].x * total_SP_Y) + current_points[0].y].AddEntity(addMe->GetParentHandle());
		SP_Grids[(current_points[1].x * total_SP_Y) + current_points[1].y].AddEntity(addMe->GetParentHandle());
		SP_Grids[(current_points[2].x * total_SP_Y) + current_points[2].y].AddEntity(addMe->GetParentHandle());
		SP_Grids[(current_points[3].x * total_SP_Y) + current_points[3].y].AddEntity(addMe->GetParentHandle());
	}
}

/********************************************************************************
Added entity as children
********************************************************************************/
void GridMap::AddChildren(Entity* child)
{
	//check if children is a layer--------------------------------------//
	if (EntityManager::CheckEntityType<GridLayer>(child))
	{
		static_cast<GridLayer*>(child)->Added_toGridMap(tileScale, totalX_tiles, totalY_tiles);
	}
	else
	{
		//Get their list of components
		CU::entityMan.GetEntityComp(child->GetHandle());

		//look through for children's SP comp------------------------------//
		for (int i = 0; i < CU::entityMan.compList.size(); ++i)
		{
			//If matches SP Comp--------------------//
			if (EntityManager::CheckCompType<SP_Comp>(CU::entityMan.compList[i]))
			{
				spComp_List.push_back(static_cast<SP_Comp*>(CU::entityMan.compList[i]));
				break;
			}
		}
	}

	Entity::AddChildren(child);	//add as children ONLY if got SP comp
}

/********************************************************************************
Getters
********************************************************************************/
int GridMap::Get_TotalTilesX(){ return totalX_tiles; }
int GridMap::Get_TotalTilesY(){ return totalY_tiles; }
float GridMap::Get_TileScale(){ return tileScale; }