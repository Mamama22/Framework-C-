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
	player_AABB.resize(2);
	player_AABB[0] = InitCharacter(&player, Vector3(-100, -100, 0), Vector3(20, 20, 1), CU::shared.quad_2);
	//player_AABB[1] = InitCharacter(&pickup, Vector3(220, -100, 0), Vector3(20, 20, 1), CU::shared.quad_2);

	//Add player and entity to gridmap--------------------------------------//
	gridmap->AddChildren(player);

	////Obstacles-------------------------------------------------//
	//Entity* ob;
	//obstacle_AABB.resize(2);
	//obstacle_AABB[0] = InitCharacter(&ob, Vector3(-100, 60, 0), Vector3(30, 150, 1), CU::shared.quad_1);
	//obstacle_AABB[1] = InitCharacter(&ob, Vector3(220, -50, 0), Vector3(170, 70, 1), CU::shared.quad_1);
}

/********************************************************************************
Character
********************************************************************************/
AABB* Scene_Boxhead::InitCharacter(Entity** pointer, Vector3 pos, Vector3 box_scale, Mesh* boxMesh)
{
	*pointer = new Entity;
	(*pointer)->Init(pos, Vector3(1.f, 1.f, 1.f));
	(*pointer)->SetActive(true);

	//Add renderer------------------------------------------------//
	Render_InWorld* mama = new Render_InWorld;
	mama->Init("fuck u", CU::shared.playerQuad, pos, Vector3(40.f, 40.f, 1.f));	//assign available renderer
	mama->SetActive(true);
	(*pointer)->AddComponent(mama);

	//Add AABB----------------------------------------------------//
	AABB* boxy = new AABB;
	boxy->Init("fuck u", boxMesh, pos, box_scale);
	boxy->SetActive(true);
	(*pointer)->AddComponent(boxy);

	return boxy;
}

/********************************************************************************
Init gridmap
********************************************************************************/
void Scene_Boxhead::InitGridmap()
{
	//gridmap---------------------------------------------------------//
	gridmap = new GridMap;
	gridmap->Init(Vector3(-400, -300, 0), TEX_MC_TILEMAP, 11.f, 50, 50, 5, 5, 2, 2, 5, 5);
	gridmap->SetActive(true);

	//modify times----------------------------//
	for (int x = 0; x < gridmap->Get_TotalTilesX(); ++x)
	{
		for (int y = 0; y < gridmap->Get_TotalTilesY(); ++y)
		{
			int tileType = rand() % gridmap->Get_TilemapSize();	//tilemap size is 4
			gridmap->ModifyTile(tileType, x, y);
		}
	}
	gridmap->RecalculateMesh();
}

/********************************************************************************
Add child entity to parent entity
********************************************************************************/
void Scene_Boxhead::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

/********************************************************************************
Update player input
********************************************************************************/
void Scene_Boxhead::UpdatePlayerInput()
{
	if (CU::input.IsKeyPressed(Input::W))
		player->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		player->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		player->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		player->Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		gridmap->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		gridmap->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		gridmap->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		gridmap->Translate(Vector3(2.f, 0, 0));

	//player's rotation--------------------------------------//
	/*if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		player->Rotate(2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		player->Rotate(-2.f, Vector3(0, 0, 1));*/

	////pick up's rotation--------------------------------------//
	//if (CU::input.IsKeyPressed(Input::V))
	//	pickUp->Rotate(2.f, Vector3(0, 0, 1));
	//if (CU::input.IsKeyPressed(Input::B))
	//	pickUp->Rotate(-2.f, Vector3(0, 0, 1));

	//Add entities to main test entity------------------------------ -//
	/*if (CU::input.IsKeyReleased(Input::N))
		AddAsChild(*player, *pickup);*/
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
	CU::view.RenderMesh(*CU::shared.axes, 1.f);
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