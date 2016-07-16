#include "Scene_SAT_test.h"

Scene_SAT_Test::Scene_SAT_Test()
{
}


Scene_SAT_Test::~Scene_SAT_Test(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_SAT_Test::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//Meshes---------------------------------------//
	sphere = MeshBuilder::GenerateSphere(Color(0, 255, 0), 36, 36, 0.5f);
	quad = MeshBuilder::GenerateQuad(Color(0, 255, 0), 1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(56.f, 255.f, 46.f));
	line_3 = MeshBuilder::GenerateLine(Color(245.f, 76.f, 245.f));
	line_axis = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f), false);

	//Init Shapes--------------------------------------//
	Init_Shapes();
}

/********************************************************************************
Init shapes
********************************************************************************/
void Scene_SAT_Test::Init_Shapes()
{
	testShape.Init("farkle");

	//rectangle shape-------------------------------------------//
	testShape.AddFace(Vector3(-100.f, -100.f, 0.f), Vector3(100.f, -100.f, 0.f));
	testShape.AddFace(Vector3(100.f, -100.f, 0.f), Vector3(100.f, 100.f, 0.f));
	testShape.AddFace(Vector3(100.f, 100.f, 0.f), Vector3(-100.f, 100.f, 0.f));

	testShape.AddFace(Vector3(-100.f, 100.f, 0.f), Vector3(-200.f, 50.f, 0.f));

	testShape.AddFace(Vector3(-200.f, 50.f, 0.f), Vector3(-100.f, -100.f, 0.f));
}

/********************************************************************************
Run
********************************************************************************/
void Scene_SAT_Test::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	Update_Shapes();
}

/********************************************************************************
update shapes
********************************************************************************/
void Scene_SAT_Test::Update_Shapes()
{
	//translation----------------------------------------//
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		testShape.Translate(Vector3(0, 2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		testShape.Translate(Vector3(0, -2.f, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		testShape.Translate(Vector3(-2.f, 0, 0));
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		testShape.Translate(Vector3(2.f, 0, 0));

	//rotation----------------------------------------//
	if (CU::input.IsKeyPressed(Input::K))
		testShape.Rotate(2.f);
	if (CU::input.IsKeyPressed(Input::L))
		testShape.Rotate(-2.f);
}

/********************************************************************************
Draw in world
********************************************************************************/
void Scene_SAT_Test::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_SAT_Test::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*axes);

	//Draw shapes------------------------------------------------//
	Draw_Shapes();

	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//FPS---------------------------------------------------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	//CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(242.f, 242.f, 7.f));
}

/********************************************************************************
Draw shapes
********************************************************************************/
void Scene_SAT_Test::Draw_Shapes()
{
	testShape.Draw();
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_SAT_Test::Exit()
{
	delete sphere;
	delete quad;
	delete axes;
	delete line_1;
	delete line_2;
	delete line_3;
	delete line_axis;

	//Call parent--------------------------------------//
	Scene::Exit();
}