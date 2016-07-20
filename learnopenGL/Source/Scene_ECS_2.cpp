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
	AddRendererToTest(testEnt[0], CU::shared.sphere, Vector3(-100.f, -1.f, 0.f), Vector3(70.f, 70.f, 70.f));
	AddRendererToTest(testEnt[1], CU::shared.sphere_1, Vector3(-100.f, 80.f, 0.f), Vector3(70.f, 70.f, 70.f));
	AddRendererToTest(testEnt[2], CU::shared.quad, Vector3(100.f, 100.f, 0.f), Vector3(70.f, 70.f, 70.f));

	//Add collider to entity 0------------------------------------------------------//
	Shape_List[0].Init("farkle", Vector3(-50.f, -1.f, 0.f));
	Shape_List[0].AddPoint(Vector3(-80.5f, -15.f, 0.f));
	Shape_List[0].AddPoint(Vector3(80.5f, -45.f, 0.f));
	Shape_List[0].AddPoint(Vector3(80.f, 45.f, 0.f));
	Shape_List[0].AddPoint(Vector3(-80.f, 25.f, 0.f));

	Shape_List[1].Init("farkle", Vector3(-100.f, -1.f, 0.f));
	Shape_List[1].AddPoint(Vector3(-80.5f, -15.f, 0.f));
	Shape_List[1].AddPoint(Vector3(80.5f, -45.f, 0.f));
	Shape_List[1].AddPoint(Vector3(80.f, 45.f, 0.f));
	Shape_List[1].AddPoint(Vector3(-80.f, 25.f, 0.f));

	Shape_List[2].Init("farkle", Vector3(-100.f, -1.f, 0.f));
	Shape_List[2].AddPoint(Vector3(-80.5f, -15.f, 0.f));
	Shape_List[2].AddPoint(Vector3(80.5f, -45.f, 0.f));
	Shape_List[2].AddPoint(Vector3(80.f, 45.f, 0.f));
	Shape_List[2].AddPoint(Vector3(-80.f, 25.f, 0.f));

	//calculate faces for this shape--------------------------//
	Shape_List[0].CalculateFaces();
	Shape_List[0].SetActive(true);
	testEnt[0].AddComponent(&Shape_List[0]);

	Shape_List[1].CalculateFaces();
	Shape_List[1].SetActive(true);
	testEnt[1].AddComponent(&Shape_List[1]);

	Shape_List[2].CalculateFaces();
	Shape_List[2].SetActive(true);
	testEnt[2].AddComponent(&Shape_List[2]);

	colliderCounter = 3;
}

/********************************************************************************
Run
********************************************************************************/
void Scene_ECS_2::AddRendererToTest(Entity& addToMe, Mesh* mesh, Vector3 pos, Vector3 scale)
{
	Render_InWorld_List[rendererCounter].Init("fuck u", mesh, pos, scale);	//assign available renderer
	Render_InWorld_List[rendererCounter].SetActive(true);
	addToMe.AddComponent(&Render_InWorld_List[rendererCounter]);
	rendererCounter++;
}

/********************************************************************************
Run
********************************************************************************/
void Scene_ECS_2::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//
	//Control---------------------------------------------------------------------//
	//testEnt.GetComp<Render_OnScreen>("renderer")->Translate(Vector3(0, 1, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		testEnt[0].Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		testEnt[0].Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		testEnt[0].Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		testEnt[0].Translate(Vector3(2.f, 0, 0));

	if (CU::input.IsKeyPressed(Input::C))
	{
		//custom rotate---------------------------//
		//testEnt.Rotate(10.f, Vector3(0, 1, 0));
		testEnt[0].transform.Start_CustomTrans(true);
		//testEnt.transform.Custom_Translate(Vector3(-47.5f, 0.f, 0.f));
		testEnt[0].transform.Custom_Rotate(2.f, Vector3(0, 0, 1));
		//testEnt.transform.Custom_Translate(Vector3(47.5f, 0.f, 0.f));
		testEnt[0].transform.End_CustomTrans();
	}
	if (CU::input.IsKeyPressed(Input::B))
		testEnt[0].Rotate(2.f, Vector3(0, 0, 1));


	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].CalculateTRS();

	//stage 3: Update with changes ===========================================================//

	//Entity update------------------------------------------------------//
	for (int i = 0; i < entityCounter; ++i)
		testEnt[i].Update();

	//Comp update-----------------------------------------------------//
	for (int i = 0; i < TOTAL_RENDERER; ++i)
		Render_InWorld_List[i].Update();
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
	//Renderer update (Draw)---------------------------------------------//
	for (int i = 0; i < TOTAL_RENDERER; ++i)
		Render_InWorld_List[i].Draw();

	for (int i = 0; i < TOTAL_SHAPE; ++i)
		Shape_List[i].Draw();


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