#include "Scene_Test.h"

Scene_Test::Scene_Test()
{
}


Scene_Test::~Scene_Test(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Test::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//base entity--------------------------------//
	base = new Entity;
	base->Init(Vector3(0,0,0), Vector3(1.f, 1.f, 1.f));
	base->SetActive(true);

	//character----------------------------------------------//
	InitCharacter(&player, Vector3(0, 0, 0), Vector3(100, 100, 1));
	player_AABB.push_back( AddAABB(player, Vector3(-75, 0, 0), Vector3(50, 50, 1), CU::shared.quad_1) );
	player_AABB.push_back( AddAABB(player, Vector3(75, 0, 0), Vector3(50, 50, 1), CU::shared.quad_1) );

	InitCharacter(&pickup_1, Vector3(-100, 0, 0), Vector3(25, 25, 1));
	player_AABB.push_back(AddAABB(pickup_1, Vector3(-100, 0, 0), Vector3(25, 25, 1), CU::shared.quad_1));

	InitCharacter(&pickup_2, Vector3(-200, -100, 0), Vector3(25, 25, 1));
	player_AABB.push_back(AddAABB(pickup_2, Vector3(-200, -100, 0), Vector3(25, 25, 1), CU::shared.quad_1));

	//Init obstacles------------------------------------------//
	obstacle_AABB.push_back(AddAABB(base, Vector3(-175, -200, 0), Vector3(50, 50, 1), CU::shared.quad_1));
	obstacle_AABB.push_back(AddAABB(base, Vector3(175, 200, 0), Vector3(50, 50, 1), CU::shared.quad_1));
}

/********************************************************************************
Init character
1) base character entity + added renderer
********************************************************************************/
void Scene_Test::InitCharacter(Entity** pointer, Vector3 pos, Vector3 scale)
{
	*pointer = new Entity;
	(*pointer)->Init(pos, Vector3(1.f, 1.f, 1.f));

	//Add renderer------------------------------------------------//
	Render_InWorld* mama = new Render_InWorld;
	mama->Init("fuck u", CU::shared.quad_2, pos, scale);	//assign available renderer
	(*pointer)->AddComponent(mama);

	(*pointer)->SetActive(true);

	base->AddChildren(*pointer);
}

/********************************************************************************
Add AABB bound to player
********************************************************************************/
AABB* Scene_Test::AddAABB(Entity* player, Vector3 pos, Vector3 scale, Mesh* boxMesh)
{
	//Add AABB----------------------------------------------------//
	AABB* boxy = new AABB;
	boxy->Init("fuck u", boxMesh, pos, scale, false);
	player->AddComponent(boxy);

	return boxy;
}

/********************************************************************************
Add child entity to parent entity
********************************************************************************/
void Scene_Test::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

void Scene_Test::RemoveChild(Entity& parent, Entity& child)
{
	parent.RemoveChildren(&child);
}

/********************************************************************************
Update player input
********************************************************************************/
void Scene_Test::UpdatePlayerInput()
{
	if (CU::input.IsKeyPressed(Input::W))
		player->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		player->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		player->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		player->Translate(Vector3(2.f, 0, 0));

	/*if (CU::input.IsKeyPressed(Input::ARROW_UP))
		pickup->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		pickup->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		pickup->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		pickup->Translate(Vector3(2.f, 0, 0));*/

	//player's rotation--------------------------------------//
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
	player->Rotate(2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
	player->Rotate(-2.f, Vector3(0, 0, 1));

	////pick up's rotation--------------------------------------//
	//if (CU::input.IsKeyPressed(Input::V))
	//	pickup->Rotate(2.f, Vector3(0, 0, 1));
	//if (CU::input.IsKeyPressed(Input::B))
	//	pickup->Rotate(-2.f, Vector3(0, 0, 1));

	//Add entities to main test entity-------------------------------//
	if (CU::input.IsKeyReleased(Input::N))	//pickup 1 add to player
	{
		RemoveChild(*base, *pickup_1);
		AddAsChild(*player, *pickup_1);
	}
	if (CU::input.IsKeyReleased(Input::M))	//pickup 2 add to pickup_1
	{
		RemoveChild(*base, *pickup_2);
		AddAsChild(*pickup_1, *pickup_2);
	}

	if (CU::input.IsKeyReleased(Input::O))	//remove pickup 1
	{
		RemoveChild(*player, *pickup_1);
		AddAsChild(*base, *pickup_1);
	}
	if (CU::input.IsKeyReleased(Input::P))	//remove pickup 2
	{
		RemoveChild(*pickup_1, *pickup_2);
		AddAsChild(*base, *pickup_2);
	}
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Test::Run_Stage1()
{
	//Call parent--------------------------------------//
	Scene::Run_Stage1();

	UpdatePlayerInput();
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Test::Run_Stage3()
{
	Scene::Run_Stage3();

	//collision check-------------------------------//
	for (int i = 0; i < player_AABB.size(); ++i)
	{
		for(int t = 0; t < obstacle_AABB.size(); ++t)
		{
			player_AABB[i]->CollisionCheck(*obstacle_AABB[t]);
		}
	}
}

/********************************************************************************
Draw in world (3D)
********************************************************************************/
void Scene_Test::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen (2D)
********************************************************************************/
void Scene_Test::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::shared.axes->Render();
}

/********************************************************************************
Draw GUI
********************************************************************************/
void Scene_Test::DrawGUI()
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
	CU::view.RenderText("TEST SCENE", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Test::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}