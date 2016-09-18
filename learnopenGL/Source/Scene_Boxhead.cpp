#include "Scene_Boxhead.h"

Scene_Boxhead::Scene_Boxhead()
{
}


Scene_Boxhead::~Scene_Boxhead(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Boxhead::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//gridmap--------------------------------------------------------//
	InitGridmap();

	//character----------------------------------------------//
	player_AABB.resize(3);
	player_AABB[0] = InitCharacter(&player, Vector3(-100, -100, 0), Vector3(20, 20, 1), CU::shared.quad_2, AImap->GetHandle(), false);
	player_AABB[1] = InitCharacter(&pickup, Vector3(120, -100, 0), Vector3(20, 20, 1), CU::shared.quad_2, AImap->GetHandle(), true);
	player_AABB[2] = InitCharacter(&pickup_1, Vector3(120, 100, 0), Vector3(20, 20, 1), CU::shared.quad_2, AImap->GetHandle(), false);

	//Add player and entity to gridmap--------------------------------------//
	gridmap->AddChildren(player);
	gridmap->AddChildren(pickup);
	gridmap->AddChildren(pickup_1);

	//Obstacles-------------------------------------------------//
	/*Entity* ob;
	obstacle_AABB.resize(2);
	obstacle_AABB[0] = InitCharacter(&ob, Vector3(-100, 60, 0), Vector3(30, 150, 1), CU::shared.quad_1);
	obstacle_AABB[1] = InitCharacter(&ob, Vector3(220, -50, 0), Vector3(170, 70, 1), CU::shared.quad_1);*/
}

/********************************************************************************
Character
********************************************************************************/
AABB* Scene_Boxhead::InitCharacter(Character** pointer, Vector3 pos, Vector3 box_scale, Mesh* boxMesh, int AI_map_ID, bool displayPath)
{
	*pointer = new Character;
	(*pointer)->Init(pos, Vector3(1.f, 1.f, 1.f), AI_map_ID, displayPath);

	//Add renderer------------------------------------------------//
	Render_InWorld* mama = new Render_InWorld;
	mama->Init("fuck u", CU::shared.playerQuad, pos, Vector3(10.f, 10.f, 1.f));	//assign available renderer
	(*pointer)->AddComponent(mama);

	//Add AABB----------------------------------------------------//
	AABB* boxy = new AABB;
	boxy->Init("fuck u", boxMesh, pos, box_scale);
	(*pointer)->AddComponent(boxy);

	//Add SP comp---------------------------------------------------//
	SP_Comp* spcomp = new SP_Comp;
	spcomp->Init("asdasd", pos, box_scale);
	(*pointer)->AddComponent(spcomp);

	(*pointer)->SetActive(true);

	return boxy;
}

/********************************************************************************
Init gridmap
********************************************************************************/
void Scene_Boxhead::InitGridmap()
{
	//gridmap---------------------------------------------------------//
	gridmap = new GridMap;
	//pos, mesh, tilescale, total tiles X, total tiles Y, total X grids, total Y grids, tilemap size X, tilemap size Y, total SP X, total SP Y
	gridmap->Init(Vector3(-400, -300, 0), TILEMAP_MC, 20.f, 25, 25, 1, 1, 2, 2, 2, 2);
	gridmap->SetActive(true);

	//modify times----------------------------//
	for (int x = 0; x < gridmap->Get_TotalTilesX(); ++x)
	{
		for (int y = 0; y < gridmap->Get_TotalTilesY(); ++y)
		{
			int tileType = rand() % CU::shared.tilemapList[gridmap->Get_TilemapEnum()].total_tiles();	//tilemap size is 4
			gridmap->ModifyTile(tileType, x, y);
		}
	}
	gridmap->RecalculateMesh();


	//ai map---------------------------------------------------//
	vector< vector<int> > pathMap;
	pathMap.resize(25);

	//the inital map-----------------------------------------//
	for (int x = 0; x < 25; ++x)
	{
		pathMap[x].resize(25);

		if (x >= 4 && x <= 20)
		{
			for (int y = 0; y < 25; ++y)
			{
				if (y >= 4 && y <= 20)
					pathMap[x][y] = 0;
				else
					pathMap[x][y] = 1;
			}
		}
		else
		{
			for (int y = 0; y < 25; ++y)
			{
				pathMap[x][y] = 1;
			}
		}
	}

	AImap = new AI_Map;

	AImap->Init(gridmap->transform.pos, pathMap, gridmap->Get_TileScale(), gridmap->Get_TotalTilesX(), gridmap->Get_TotalTilesY());
	AImap->SetActive(true);
	gridmap->AddChildren(AImap);

	////test AI_Comp-----------------------------------------------------------------//
	//test_AI = new AI_Comp;
	//test_AI->Init("ASD", AImap->GetHandle(), true);
	//AImap->AddComponent(test_AI);

	////test find path-----------------//
	//test_AI->findPath(2, 2, 24, 22);
}

/********************************************************************************
Add child entity to parent entity
********************************************************************************/
void Scene_Boxhead::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

void Scene_Boxhead::RemoveChild(Entity& parent, Entity& child)
{
	parent.RemoveChildren(&child);
}

/********************************************************************************
Update player input
********************************************************************************/
void Scene_Boxhead::UpdatePlayerInput()
{
	if (CU::input.IsKeyPressed(Input::W))
		gridmap->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		gridmap->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		gridmap->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		gridmap->Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		player->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		player->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		player->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		player->Translate(Vector3(2.f, 0, 0));

	//AI path finding---------------------------------------//
	if (CU::input.IsKeyReleased(Input::K))
		pickup->Test_FindPath();

	//player's rotation--------------------------------------//
	/*if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		player->Rotate(2.f, Vector3(0, 0, 1));
		if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		player->Rotate(-2.f, Vector3(0, 0, 1));*/

	////pick up's rotation--------------------------------------//
	//if (CU::input.IsKeyPressed(Input::V))
	//	pickup->Rotate(2.f, Vector3(0, 0, 1));
	//if (CU::input.IsKeyPressed(Input::B))
	//	pickup->Rotate(-2.f, Vector3(0, 0, 1));

	//Add entities to main test entity-------------------------------//
	if (CU::input.IsKeyReleased(Input::N))
	{
		RemoveChild(*gridmap, *pickup);
		AddAsChild(*player, *pickup);
	}
	if (CU::input.IsKeyReleased(Input::M))
	{
		RemoveChild(*gridmap, *pickup_1);
		AddAsChild(*pickup, *pickup_1);
	}

	if (CU::input.IsKeyReleased(Input::O))
	{
		RemoveChild(*player, *pickup);
		AddAsChild(*gridmap, *pickup);
	}
	if (CU::input.IsKeyReleased(Input::P))
	{
		RemoveChild(*pickup, *pickup_1);
		AddAsChild(*gridmap, *pickup_1);
	}
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Boxhead::Run_Stage1()
{
	//Call parent--------------------------------------//
	Scene::Run_Stage1();

	UpdatePlayerInput();
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Boxhead::Run_Stage3()
{
	Scene::Run_Stage3();

	////collision check-------------------------------//
	//for (int i = 0; i < player_AABB.size(); ++i)
	//{
	//	for(int t = 0; t < obstacle_AABB.size(); ++t)
	//	{
	//		player_AABB[i]->CollisionCheck(*obstacle_AABB[t]);
	//	}
	//}
}

/********************************************************************************
Draw in world (3D)
********************************************************************************/
void Scene_Boxhead::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen (2D)
********************************************************************************/
void Scene_Boxhead::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	//CU::shared.axes->Render();
}

/********************************************************************************
Draw GUI
********************************************************************************/
void Scene_Boxhead::DrawGUI()
{
	//Text----------------------------------------------------//
	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));

	ss.str("");
	ss << "TRS cal count: " << Transformation::TRS_cal_count;
	CU::view.RenderText(ss.str(), Vector2(-390.f, 190.f), 0.55f, Color(24.f, 196.f, 87.f));
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Boxhead::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}