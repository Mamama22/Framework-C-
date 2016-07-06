#include "Scene_ECS.h"

Scene_ECS::Scene_ECS()
{
	rendererCounter = 0;
	meshType = 0;
	added1 = added2 = false;
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

	//Init entity-----------------------------------------------------------//
	testEnt.Init(Vector3(-10.f, -1.f, 0.f));
	testEnt.Init(Vector3(-10.f, 5.f, 0.f));
	testEnt.Init(Vector3(10.f, 10.f, 0.f));

	//Add renderer to test----------------------------------------------------//
	AddRendererToTest(testEnt, offset, true);
	AddRendererToTest(testEnt_1, offset1, true);
	AddRendererToTest(testEnt_2, offset2, true);
}

/********************************************************************************
Add renderer comp to a entity
********************************************************************************/
void Scene_ECS::AddRendererToTest(Entity& addToMe, Vector3 offset, bool first)
{
	Vector3 RendererScale;
	if (first)
		RendererScale.Set(3.f, 3.f, 3.f);
	else
		RendererScale.Set(1.5f, 1.5f, 1.5f);

	Vector3 newPos = addToMe.transform.pos + offset;
	ostringstream ss;
	ss << "renderer" << rendererCounter;

	if (meshType == 0)
		Render_InWorld_List[rendererCounter].Init(ss.str().c_str(), sphere, newPos, RendererScale);	//assign available renderer
	else if (meshType == 1)
		Render_InWorld_List[rendererCounter].Init(ss.str().c_str(), sphere, newPos, RendererScale);	//assign available renderer

	meshType++;
	if (meshType > 1)
		meshType = 0;

	Render_InWorld_List[rendererCounter].SetActive(true);
	addToMe.AddComponent(&Render_InWorld_List[rendererCounter]);
	++rendererCounter;
}

/********************************************************************************
Add entity to parent
********************************************************************************/
void Scene_ECS::AddTestEnt1()
{
	if (added1)
		return;
	added1 = true;
	testEnt.AddChildren(&testEnt_1);
}
void Scene_ECS::AddTestEnt2()
{
	if (added2)
		return;
	added2 = true;
	testEnt_1.AddChildren(&testEnt_2);
}

/********************************************************************************
Run
********************************************************************************/
void Scene_ECS::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Add new comp------------------------------------------------//
	if (CU::input.IsKeyReleased(Input::I))
	{
		AddRendererToTest(testEnt, offset, false);
		offset += Vector3(1.5f, 1.5f, 0);
	}
	if (CU::input.IsKeyReleased(Input::O))
	{
		AddRendererToTest(testEnt_1, offset1, false);
		offset1 += Vector3(0, 1.5f, 0);
	}
	if (CU::input.IsKeyReleased(Input::P))
	{
		AddRendererToTest(testEnt_2, offset2, false);
		offset2 += Vector3(-0, -1.5f, 0);
	}

	//Add entities to main test entity-------------------------------//
	if (CU::input.IsKeyReleased(Input::N))
		AddTestEnt1();
	if (CU::input.IsKeyReleased(Input::M))
		AddTestEnt2();

	//Control---------------------------------------------------------------------//
	//testEnt.GetComp<Render_OnScreen>("renderer")->Translate(Vector3(0, 1, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		testEnt.Translate(Vector3(0, 0.15f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		testEnt.Translate(Vector3(0, -0.15f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		testEnt.Translate(Vector3(-0.15f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		testEnt.Translate(Vector3(0.15f, 0, 0));
	if (CU::input.IsKeyPressed(Input::C))
		testEnt.Rotate(5.f);
	if (CU::input.IsKeyPressed(Input::B))
		testEnt_1.Rotate(5.f);


	//Entity update------------------------------------------------------//
	testEnt.Update();
	testEnt_1.Update();
	testEnt_2.Update();
}

/********************************************************************************
Draw in worldq
********************************************************************************/
void Scene_ECS::DrawInWorld()
{
	CU::view.UseShader(View::LIGHT_SHADER);	//use light shader

	//Renderer update (Draw)---------------------------------------------//
	for (int i = 0; i < TOTAL_RENDERER; ++i)
		Render_InWorld_List[i].Update();

	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader

	//Point light----------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(-2.f, 2.f, 2.5f);
	CU::view.Scale(0.3f, 0.3f, 0.3f);
	CU::view.RenderMesh(*sphere);

	//spot light------------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(-12.f, 1.f, 7.f);
	CU::view.Scale(0.2f, 0.2f, 0.2f);
	CU::view.RenderMesh(*sphere);

	CU::view.SetIdentity();
	CU::view.Translate(13.f, 1.f, 6.f);
	CU::view.Scale(0.2f, 0.2f, 0.2f);
	CU::view.RenderMesh(*sphere);

	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*axes);
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_ECS::DrawOnScreen()
{
	//CU::view.UseShader(View::BASIC_SHADER);	//use basic shader

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
void Scene_ECS::Exit()
{
	delete sphere;
	delete quad;
	delete axes;

	//Call parent--------------------------------------//
	Scene::Exit();
}