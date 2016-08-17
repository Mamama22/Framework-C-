#include "Scene_Game.h"
int shapeTypeCount = 0;

Scene_Game::Scene_Game()
{
	entityCounter = rendererCounter = colliderCounter = 0;
}


Scene_Game::~Scene_Game(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Game::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	colliderCounter = entityCounter = 0;

	//Player-----------------------------------------------------------//
	InitPlayer();

	//obstacles--------------------------------------------------------//
	InitObstacles();
}

/********************************************************************************
init obstacles
********************************************************************************/
void Scene_Game::InitObstacles()
{
	int index = entityCounter;
	obStart_index = index;

	testEnt[index].Init(Vector3(250.f, -200.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(250.f, 200.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-250.f, 200.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-250.f, -200.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-100.f, -100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	//
	testEnt[index].Init(Vector3(150.f, -150.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(150.f, 100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-150.f, 10.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-150.f, -100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-110.f, -50.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[1], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	obEnd_index = index;
	entityCounter = index;
}

/********************************************************************************
Init player
********************************************************************************/
void Scene_Game::InitPlayer()
{
	int index = entityCounter;
	player = &testEnt[index];
	player->Init(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f));

	//Add player collider------------------------------------------//
	Shape_List[colliderCounter].Init("farkle2", player->transform.pos);

	Shape_List[colliderCounter].AddPoint(Vector3(-20.0f, -20.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(20.f, -20.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(20.f, 20.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(-20.f, 20.f, 0.f));

	Shape_List[colliderCounter].CalculateFaces();
	Shape_List[colliderCounter].SetActive(true);
	player->AddComponent(&Shape_List[colliderCounter]);
	colliderCounter++;

	AddRendererToEntity(testEnt[index], CU::shared.playerQuad, Vector3(40.f, 40.f, 1.f));
	entityCounter += 1;

	//gun--------------------------------------------------------//
	rayLength = detectionRange = 1000.f;
	intersectedPlane = false;
}

/********************************************************************************
Utilities
********************************************************************************/
void Scene_Game::AddRendererToEntity(Entity& addToMe, Mesh* mesh, Vector3 scale)
{
	Vector3 pos = addToMe.transform.GetPos();

	Render_InWorld_List[rendererCounter].Init("fuck u", mesh, pos, scale);	//assign available renderer
	Render_InWorld_List[rendererCounter].SetActive(true);
	addToMe.AddComponent(&Render_InWorld_List[rendererCounter]);
	rendererCounter++;
}

void Scene_Game::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

/********************************************************************************
Add random shapes to entity
********************************************************************************/
void Scene_Game::AddShape(Entity& addToMe)
{
	Shape_List[colliderCounter].Init("farkle2", addToMe.transform.pos);

	//Shape type---------------------------------------//
	if (shapeTypeCount == 0)
	{
		Shape_List[colliderCounter].AddPoint(Vector3(-40.0f, -40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, -40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, 40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-40.f, 40.0f, 0.f));
	}
	else if (shapeTypeCount == 1)
	{
		Shape_List[colliderCounter].AddPoint(Vector3(-40.0f, -40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, -40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, 40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-40.f, 40.0f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-80.f, 0.0f, 0.f));
	}
	else if (shapeTypeCount == 2)
	{
		Shape_List[colliderCounter].AddPoint(Vector3(-60.0f, -40.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-40.0f, -80.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, -20.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(50.f, 20.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-40.f, 40.0f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-80.f, 20.0f, 0.f));
	}
	else if (shapeTypeCount == 3)
	{
		Shape_List[colliderCounter].AddPoint(Vector3(-20.0f, -20.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-10.0f, -60.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(40.f, 20.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(50.f, 70.f, 0.f));
		Shape_List[colliderCounter].AddPoint(Vector3(-40.f, 40.0f, 0.f));
	}

	//calculate faces for this shape--------------------------//
	Shape_List[colliderCounter].CalculateFaces();

	//Add--------------------------//
	Shape_List[colliderCounter].SetActive(true);
	addToMe.AddComponent(&Shape_List[colliderCounter]);
	colliderCounter++;
	shapeTypeCount++;

	if (shapeTypeCount >= 4)
		shapeTypeCount = 0;
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Game::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
	{
		testEnt[i].PreUpdate();
	}

	//reset by parent rotate------------------------------------------------------//
	for (int i = 0; i < colliderCounter; ++i)
		Shape_List[colliderCounter].PreUpdate();

	//Control---------------------------------------------------------------------//
	//Input (Pre-update)
	UpdatePlayerInput();


	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();


	//stage 3: Update with changes ===========================================================//

	//check gun hit
	UpdateRayGun();

	//collision check-------------------------------//
	for (int i = obStart_index; i < obEnd_index; ++i)
		Shape_List[0].CollisionCheck_2(Shape_List[i]);

	//Entity update------------------------------------------------------//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].Update();

	//Comp update-----------------------------------------------------//
	for (int i = 0; i < TOTAL_RENDERER; ++i)
	{
		Render_InWorld_List[i].Update();
	}
	for (int i = 0; i < TOTAL_SHAPE; ++i)
		Shape_List[i].Update();


	//Stage 4: 2nd TRS calculations for Entity and Comp (For those with changes) ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();
}

/********************************************************************************
Update player
********************************************************************************/
void Scene_Game::UpdatePlayerInput()
{
	if (CU::input.IsKeyPressed(Input::W))
		player->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		player->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		player->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		player->Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		player->Rotate(2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		player->Rotate(-2.f, Vector3(0, 0, 1));
}

/********************************************************************************
update gun
********************************************************************************/
void Scene_Game::UpdateRayGun()
{
	intersectedPlane = false;
	Vector3 playerPos = player->transform.GetPos();
	Vector3 tmp_intersectPos;
	float shortestLen = 100000000000.f;

	for (int i = obStart_index; i < obEnd_index; ++i)
	{
		bool gotIntersect = Shape_List[i].CheckLineIntersection(playerPos, player->transform.angle + 90.f, tmp_intersectPos);

		if (gotIntersect)
		{
			intersectedPlane = true;
			if ((tmp_intersectPos - playerPos).LengthSquared() < shortestLen)
			{
				shortestLen = (tmp_intersectPos - playerPos).LengthSquared();
				gunIntersectedPos = tmp_intersectPos;
			}
		}
	}

}

/********************************************************************************
Draw in world
********************************************************************************/
void Scene_Game::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_Game::DrawOnScreen()
{
	//Env-----------------------------------------------------//
	RenderFloor();
	RenderRay();

	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*CU::shared.axes);

	//Special: Renderer components has a draw function====================================================================//
	//Renderer Draw---------------------------------------------//
	for (int i = 0; i < TOTAL_RENDERER; ++i)
		Render_InWorld_List[i].Draw();

	//shape Draw---------------------------------------------//
	for (int i = 0; i < TOTAL_SHAPE; ++i)
		Shape_List[i].Draw();


	//Text----------------------------------------------------//
	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));
	CU::view.RenderText("ELMO", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));
}

/********************************************************************************
Render floor
800, 600
********************************************************************************/
void Scene_Game::RenderFloor()
{
	float tileScale = 100.f;

	float y = -300.f;
	
	for (; y < 300.f; y += tileScale)
	{
		for (float x = -400.f; x < 400.f; x += tileScale)
		{
			CU::view.SetIdentity();
			CU::view.Translate(x + tileScale * 0.5f, y + tileScale * 0.5f, 1.f);
			CU::view.Scale(tileScale * 0.98f, tileScale * 0.98f, 1.f);
			CU::view.RenderMesh(*CU::shared.floorQuad);
		}
	}
}

/********************************************************************************
Render ray
********************************************************************************/
void Scene_Game::RenderRay()
{
	CU::view.SetIdentity();
	CU::view.Translate(player->transform.GetPos().x, player->transform.GetPos().y, player->transform.GetPos().z);
	CU::view.Rotate(player->transform.angle + 90.f, 0, 0, 1);

	float len = rayLength;

	if (intersectedPlane)
		len = gunIntersectedPos.Length();

	CU::view.Scale(len, len, 1.f);
	CU::view.RenderMesh(*CU::shared.line_start0);
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Game::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}