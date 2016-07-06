#include "Scene_Test.h"

/********************************************************************************
Init
********************************************************************************/
void Scene_Test::Init()
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
}


/********************************************************************************
Run
********************************************************************************/
void Scene_Test::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	if (CU::input.IsKeyReleased(Input::P))
		CU::view.Reset();
}

/********************************************************************************
Draw in worldq
********************************************************************************/
void Scene_Test::DrawInWorld()
{

	CU::view.UseShader(View::LIGHT_SHADER);	//use light shader

	CU::view.SetIdentity();
	CU::view.Translate(-2.f, 2.f, 0.f);
	CU::view.Scale(2.f, 2.f, 2.f);
	CU::view.RenderMesh(*sphere);

	CU::view.SetIdentity();
	CU::view.Translate(-2.f, -2.5f, 0.f);
	CU::view.Rotate(-90.f, 1.f, 0.f, 0.f);
	CU::view.Scale(100.f, 100.f, 1.f);
	CU::view.RenderMesh(*quad);

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
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_Test::DrawOnScreen()
{
	/*CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*axes);

	CU::view.SetIdentity();
	CU::view.Translate(200.f, 100.f, 0.f);
	CU::view.Scale(100.f, 100.f, 1.f);
	CU::view.RenderMesh(*quad);*/

	CU::view.UseShader(View::TEXT_SHADER);	//use light shader

	//test quad-----------------------------------------//
	ostringstream ss;
	ss << "FPS: " << CU::fps;
	string theFPS = ss.str();
	int findDot = theFPS.find('.');
	theFPS = theFPS.substr(0, findDot);
	CU::view.RenderText(theFPS, Vector2(-390.f, 250.f), 0.5f, Color(0.f, 255.f, 255.f));
	CU::view.RenderText("OWL CITY", Vector2(50.f, -290.f), 1.f, Color(242.f, 215.f, 104.f));
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Test::Exit()
{
	delete sphere;
	delete quad;
	delete axes;

	//Call parent--------------------------------------//
	Scene::Exit();
}