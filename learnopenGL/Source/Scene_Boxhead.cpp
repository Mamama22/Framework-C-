#include "Scene_Boxhead.h"

Scene_Boxhead::Scene_Boxhead()
{
	entityCounter = rendererCounter = colliderCounter = 0;
}


Scene_Boxhead::~Scene_Boxhead(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Boxhead::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	colliderCounter = entityCounter = 0;

	//Player-----------------------------------------------------------//
	//MAKE SURE PLAYER INIT FIRST< SINCE IT'S SHAPE WILL BE PARENT
	playerStart_index = entityCounter;
	InitCharacter(&player, Vector3(0,0,0));
	InitCharacter(&pickUp, Vector3(100, -100, 0));
	playerEnd_index = entityCounter;

	//obstacles--------------------------------------------------------//
	InitObstacles();
}

/********************************************************************************
init obstacles
********************************************************************************/
void Scene_Boxhead::InitObstacles()
{
	int index = entityCounter;
	obStart_index = index;

	testEnt[index].Init(Vector3(150.f, -100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[index], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(150.f, 100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[index], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	testEnt[index].Init(Vector3(-150.f, 100.f, 0.f), Vector3(1.f, 1.f, 1.f));
	AddRendererToEntity(testEnt[index], CU::shared.quad_2, Vector3(35, 35, 35));
	AddShape(testEnt[index]);

	index++;

	obEnd_index = index;
	entityCounter = index;
}

/********************************************************************************
Init player
********************************************************************************/
void Scene_Boxhead::InitCharacter(Entity** turnMe, Vector3 pos)
{
	int index = entityCounter;

	//Assign entity from pool---------------------------------------------//
	*turnMe = &testEnt[index];
	(*turnMe)->Init(pos, Vector3(1.f, 1.f, 1.f));

	//Add collider shape to (*turnMe)------------------------------------------//
	AABB_List[colliderCounter].Init("fuck u", pos, Vector3(20.f, 20.f, 1.f));
	AABB_List[colliderCounter].SetActive(true);

	//add to it
	(*turnMe)->AddComponent(&AABB_List[colliderCounter]);

	//Component counter
	colliderCounter++;

	//Add renderer to (*turnMe)-------------------------------------------------------//
	AddRendererToEntity(testEnt[index], CU::shared.playerQuad, Vector3(40.f, 40.f, 1.f));

	entityCounter += 1;
}

/********************************************************************************
Add renderer to Entity
********************************************************************************/
void Scene_Boxhead::AddRendererToEntity(Entity& addToMe, Mesh* mesh, Vector3 scale)
{
	Vector3 pos = addToMe.transform.GetPos();

	Render_InWorld_List[rendererCounter].Init("fuck u", mesh, pos, scale);	//assign available renderer
	Render_InWorld_List[rendererCounter].SetActive(true);
	addToMe.AddComponent(&Render_InWorld_List[rendererCounter]);
	rendererCounter++;
}

/********************************************************************************
Add child entity to parent entity
********************************************************************************/
void Scene_Boxhead::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

/********************************************************************************
Add random shapes to entity
********************************************************************************/
void Scene_Boxhead::AddShape(Entity& addToMe)
{
	AABB_List[colliderCounter].Init("farkle2", addToMe.transform.pos, Vector3(50.f, 40.f, 1.f));

	//Add--------------------------//
	AABB_List[colliderCounter].SetActive(true);
	addToMe.AddComponent(&AABB_List[colliderCounter]);
	colliderCounter++;
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Boxhead::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//PRE-UPDATE ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].PreUpdate();

	//component pre-update (If have)----------------------------------------------//
	for (int i = 0; i < colliderCounter; ++i)
		AABB_List[i].PreUpdate();


	//Stage 1: States, flags and values update ===========================================================//

	//Control---------------------------------------------------------------------//
	//Input (Pre-update)
	UpdatePlayerInput();

	//Add entities to main test entity-------------------------------//
	if (CU::input.IsKeyReleased(Input::N))
		AddAsChild(*player, *pickUp);


	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();

	//stage 3: Update with changes ===========================================================//
	
	//collision check-------------------------------//
	


	//Entity update------------------------------------------------------//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].Update();

	//Comp update-----------------------------------------------------//
	for (int i = 0; i < rendererCounter; ++i)
	{
		Render_InWorld_List[i].Update();
	}
	for (int i = 0; i < colliderCounter; ++i)
	{
		AABB_List[i].Update();
	}


	//Stage 4: 2nd TRS calculations for Entity and Comp (For those with changes) ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();
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

	/*if (CU::input.IsKeyPressed(Input::ARROW_UP))
		pickUp->Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		pickUp->Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		pickUp->Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		pickUp->Translate(Vector3(2.f, 0, 0));*/

	//player's rotation--------------------------------------//
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		player->Rotate(2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		player->Rotate(-2.f, Vector3(0, 0, 1));

	//pick up's rotation--------------------------------------//
	if (CU::input.IsKeyPressed(Input::V))
		pickUp->Rotate(2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::B))
		pickUp->Rotate(-2.f, Vector3(0, 0, 1));
}

/********************************************************************************
Draw in world
********************************************************************************/
void Scene_Boxhead::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_Boxhead::DrawOnScreen()
{

	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*CU::shared.axes);

	//Special: Renderer components has a draw function====================================================================//
	//Renderer Draw---------------------------------------------//
	for (int i = 0; i < rendererCounter; ++i)
		Render_InWorld_List[i].Draw();

	//shape Draw---------------------------------------------//
	for (int i = 0; i < colliderCounter; ++i)
		AABB_List[i].Draw();

	//Text----------------------------------------------------//
	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));
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