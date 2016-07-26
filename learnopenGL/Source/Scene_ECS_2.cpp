#include "Scene_ECS_2.h"

Scene_ECS_2::Scene_ECS_2()
{
	entityCounter = rendererCounter = colliderCounter = 0;
}


Scene_ECS_2::~Scene_ECS_2(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_ECS_2::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//Lighting: Call before View init() !!!-------------------------------------//
	CU::view.AddDirLight(Vector3(0.f, -1.f, 0.f), Color(255.f, 255.f, 255.f));
	CU::view.AddPointLight(Vector3(-4.f, 4.f, 5.f));
	CU::view.AddSpotLight(Vector3(-12.f, 1.f, 7.f), Vector3(-12.5f, 0.f, 7.f), 30.f);
	CU::view.AddSpotLight(Vector3(13.f, 1.f, 6.f), Vector3(15.f, 0.f, 6.f), 35.f);

	//Init entity-----------------------------------------------------------//
	testEnt[entityCounter++].Init(Vector3(-100.f, -1.f, 0.f), Vector3(1.f, 1.f, 1.f));
	testEnt[entityCounter++].Init(Vector3(-100.f, 80.f, 0.f), Vector3(1.f, 1.f, 1.f));
	testEnt[entityCounter++].Init(Vector3(100.f, 100.f, 0.f), Vector3(1.f, 1.f, 1.f));

	//Add renderer------------------------------------------------------//
	AddRendererToTest(testEnt[0], CU::shared.quad, Vector3(-100.f + 17.5f, -1.f, 0.f), Vector3(35, 35, 35));
	AddRendererToTest(testEnt[1], CU::shared.quad_1, Vector3(-100.f + 17.5f, 80.f, 0.f), Vector3(35, 35, 35));
	AddRendererToTest(testEnt[2], CU::shared.quad_2, Vector3(100.f + 17.5f, 100.f, 0.f), Vector3(35, 35, 35));

	colliderCounter = 0;

	//Add shape---------------------------------------------------------//
	AddShape(testEnt[0]);
	AddShape(testEnt[1]);
	AddShape(testEnt[2]);
}

/********************************************************************************
Utilities
********************************************************************************/
void Scene_ECS_2::AddRendererToTest(Entity& addToMe, Mesh* mesh, Vector3 pos, Vector3 scale)
{
	Render_InWorld_List[rendererCounter].Init("fuck u", mesh, pos, scale);	//assign available renderer
	Render_InWorld_List[rendererCounter].SetActive(true);
	addToMe.AddComponent(&Render_InWorld_List[rendererCounter]);
	rendererCounter++;
}

void Scene_ECS_2::AddAsChild(Entity& parent, Entity& child)
{
	parent.AddChildren(&child);
}

void Scene_ECS_2::AddShape(Entity& addToMe)
{
	//Test shape 2---------------------------------------//
	Shape_List[colliderCounter].Init("farkle2", addToMe.transform.pos);
	Shape_List[colliderCounter].AddPoint(Vector3(-40.0f, -40.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(40.f, -40.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(40.f, 40.f, 0.f));
	Shape_List[colliderCounter].AddPoint(Vector3(-40.f, 40.0f, 0.f));

	//calculate faces for this shape--------------------------//
	Shape_List[colliderCounter].CalculateFaces();

	//Add--------------------------//
	Shape_List[colliderCounter].SetActive(true);
	addToMe.AddComponent(&Shape_List[colliderCounter]);
	colliderCounter++;
}

/********************************************************************************
Run
********************************************************************************/
void Scene_ECS_2::Run()
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
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		testEnt[0].Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		testEnt[0].Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		testEnt[0].Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		testEnt[0].Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::W))
		testEnt[1].Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::S))
		testEnt[1].Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::A))
		testEnt[1].Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		testEnt[1].Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::C))
		testEnt[0].Rotate(-2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::V))
		testEnt[1].Rotate(-2.f, Vector3(0, 0, 1));
	if (CU::input.IsKeyPressed(Input::B))
		testEnt[2].Rotate(2.f, Vector3(0, 0, 1));

	//Add entities to main test entity-------------------------------//
	if (CU::input.IsKeyReleased(Input::N))
		AddAsChild(testEnt[0], testEnt[1]);																							

	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();

	cout << "Shape 1 pos: " << Shape_List[1].transform.angle << endl;

	//stage 3: Update with changes ===========================================================//

	//collision check-------------------------------//
	if (Shape_List[0].isActive() && Shape_List[1].isActive() && Shape_List[2].isActive())
	{
		Shape_List[0].CollisionCheck_2(Shape_List[1]);
		Shape_List[0].CollisionCheck_2(Shape_List[2]);
		Shape_List[1].CollisionCheck_2(Shape_List[2]);
	}

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
Draw in world
********************************************************************************/
void Scene_ECS_2::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_ECS_2::DrawOnScreen()
{
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
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_ECS_2::Exit()
{
	//Call parent--------------------------------------//
	Scene::Exit();
}