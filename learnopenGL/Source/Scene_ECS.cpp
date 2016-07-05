#include "Scene_ECS.h"

Scene_ECS::Scene_ECS()
{
	rendererCounter = 0;
	meshType = 0;
}


Scene_ECS::~Scene_ECS(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_ECS::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//Meshes---------------------------------------//
	sphere = MeshBuilder::GenerateSphere(36, 36, 0.5f);
	quad = MeshBuilder::GenerateQuad(1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();

	//Lighting: Call before View init() !!!-------------------------------------//
	CU::view.AddDirLight(Vector3(0.f, -1.f, 0.f), Color(255.f, 255.f, 255.f));
	CU::view.AddPointLight(Vector3(-4.f, 4.f, 5.f));
	CU::view.AddSpotLight(Vector3(-12.f, 1.f, 7.f), Vector3(-12.5f, 0.f, 7.f), 30.f);
	CU::view.AddSpotLight(Vector3(13.f, 1.f, 6.f), Vector3(15.f, 0.f, 6.f), 35.f);

	//make 20 on screen renderers------------------------------------------//
	Render_OnScreen_List.resize(20);

	testEnt.Init(Vector3(-100, -100, 0));

	//Add renderer to test----------------------------------------------------//
	AddRendererToTest();
}

void Scene_ECS::AddRendererToTest()
{
	Vector3 newPos = testEnt.transform.pos + startPos;
	ss.str("");
	ss << "renderer" << rendererCounter;

	if (meshType == 0)
		Render_OnScreen_List[rendererCounter].Init(ss.str().c_str(), quad, newPos, Vector3(50, 75, 1));	//assign available renderer
	else if (meshType == 1)
		Render_OnScreen_List[rendererCounter].Init(ss.str().c_str(), sphere, newPos, Vector3(50, 75, 1));	//assign available renderer

	meshType++;
	if (meshType > 1)
		meshType = 0;

	Render_OnScreen_List[rendererCounter].SetActive(true);
	testEnt.AddComponent(&Render_OnScreen_List[rendererCounter]);
	startPos += Vector3(25, 25, 0);
	++rendererCounter;
}


/********************************************************************************
Run
********************************************************************************/
void Scene_ECS::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Add new comp------------------------------------------------//
	if (CU::input.IsKeyReleased(Input::P))
	{
		AddRendererToTest();
	}

	//Control---------------------------------------------------------------------//
	//testEnt.GetComp<Render_OnScreen>("renderer")->Translate(Vector3(0, 1, 0));
	if (CU::input.IsKeyPressed(Input::W))
		testEnt.Translate(Vector3(0, 2, 0));
	if (CU::input.IsKeyPressed(Input::S))
		testEnt.Translate(Vector3(0, -2, 0));
	if (CU::input.IsKeyPressed(Input::A))
		testEnt.Translate(Vector3(-2, 0, 0));
	if (CU::input.IsKeyPressed(Input::D))
		testEnt.Translate(Vector3(2, 0, 0));
	if (CU::input.IsKeyPressed(Input::C))
		testEnt.Rotate(2.f);
}

/********************************************************************************
Draw in worldq
********************************************************************************/
void Scene_ECS::DrawInWorld()
{

}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_ECS::DrawOnScreen()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader

	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*axes);

	//Renderer update (Draw)---------------------------------------------//
	for (int i = 0; i < Render_OnScreen_List.size(); ++i)
		Render_OnScreen_List[i].Update();


	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ss.str("");
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
void Scene_ECS::Exit()
{
	delete sphere;
	delete quad;
	delete axes;

	//Call parent--------------------------------------//
	Scene::Exit();
}