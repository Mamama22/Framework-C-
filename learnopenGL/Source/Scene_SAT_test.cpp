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
	sphere_1 = MeshBuilder::GenerateSphere(Color(217, 180, 240), 36, 36, 0.5f);
	quad = MeshBuilder::GenerateQuad(Color(0, 255, 0), 1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(56.f, 255.f, 46.f));
	line_3 = MeshBuilder::GenerateLine(Color(245.f, 76.f, 245.f));
	line_axis = MeshBuilder::GenerateLine(Color(176.f, 85.f, 19.f), false);

	//Init Shapes--------------------------------------//
	Init_Shapes();
}

/********************************************************************************
Init shapes
********************************************************************************/
void Scene_SAT_Test::Init_Shapes()
{
	//axis-------------------------------------------------//
	axisDir.Set(1,0,0);
	axisAngle = 0.f;

	//Offseting-------------------------------------------------//
	dist = 300.f;
	CU::shared.CalculateOffset(offset, axisDir, dist);

	testShape.Init("farkle");

	//rectangle shape-------------------------------------------//
	testShape.AddPoint(Vector3(-50.f, -50.f, 0.f));
	testShape.AddPoint(Vector3(50.f, -50.f, 0.f));
	testShape.AddPoint(Vector3(50.f, 50.f, 0.f));

	testShape.AddPoint(Vector3(-100.f, 25.f, 0.f));

	//calculate faces for this shape--------------------------//
	testShape.CalculateFaces();
}

/********************************************************************************
Run
********************************************************************************/
void Scene_SAT_Test::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//
	Update_Axis();
	Update_Shapes();

	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	testShape.RecalculatePoints();	//called by calculate TRS with parents but for now stand-alone since no entity adds this

	//stage 3: Update with changes ===========================================================//
	Calculate_ShapeProjections();

	//Stage 4: 2nd TRS calculations for Entity and Comp (For those with changes) ===========================================================//
	
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
Update_Axis
********************************************************************************/
void Scene_SAT_Test::Update_Axis()
{
	//rotate axis----------------------------------------//
	if (CU::input.IsKeyPressed(Input::O))
		axisAngle += 1.f;
	if (CU::input.IsKeyPressed(Input::P))
		axisAngle -= 1.f;

	if (axisAngle < 0.f)axisAngle += 360.f;
	else if (axisAngle > 360.f)axisAngle -= 360.f;

	//cal dir---------------------------------------------//
	axisDir.x = cos(Math::DegreeToRadian(axisAngle));
	axisDir.y = sin(Math::DegreeToRadian(axisAngle));

	//Offseting-------------------------------------------------//
	float xDir = cos(Math::DegreeToRadian(axisAngle + 90.f));
	float yDir = sin(Math::DegreeToRadian(axisAngle + 90.f));
	offset.Set(xDir * dist, yDir * dist, 0.f);
}

/********************************************************************************
cal shape projection
********************************************************************************/
void Scene_SAT_Test::Calculate_ShapeProjections()
{
	testShape.GetProjections(axisDir, shapeProjPoints);
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

	//projected axis------------------------------------------------//
	float angle = Vector3::getAngleFromDir(axisDir.x, axisDir.y);
	CU::shared.DrawLine_Offset(line_axis, Vector3(0, 0, 0), angle, 1000.f, 3.f, offset);

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
	Vector3 ptPos, ptDir;

	//Draw the projected points-----------------------//
	for (int i = 0; i < testShape.Get_TotalPoints(); ++i)
	{
		//Draw the point---------------------------------//
		ptPos = shapeProjPoints[i] + offset;
		CU::shared.DrawMesh(sphere_1, ptPos, 10.f);

		//Draw the line-----------------------------------//
		Vector3 line = ptPos - testShape.pointList[i].pos;
		ptDir = line.Normalized();
		float angle = Vector3::getAngleFromDir(ptDir.x, ptDir.y);
		CU::shared.DrawLine(line_3, testShape.pointList[i].pos, angle, line.Length(), 2.5f);

		//Accuracy test: successful-----------------------------------------------------//
		float dotProd = ptDir.Dot(axisDir);
		if (dotProd >= 0.0001f)
			cout << "Dot: " << ptDir.Dot(axisDir) << endl;
	}
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_SAT_Test::Exit()
{
	delete sphere;
	delete sphere_1;
	delete quad;
	delete axes;
	delete line_1;
	delete line_2;
	delete line_3;
	delete line_axis;

	//Call parent--------------------------------------//
	Scene::Exit();
}