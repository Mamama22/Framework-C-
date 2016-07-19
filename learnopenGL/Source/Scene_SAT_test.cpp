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

	sphere_2 = MeshBuilder::GenerateSphere(Color(56.f, 255.f, 46.f), 36, 36, 0.5f);
	sphere_4 = MeshBuilder::GenerateSphere(Color(223, 213, 224), 36, 36, 0.5f);

	quad = MeshBuilder::GenerateQuad(Color(0, 255, 0), 1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(56.f, 255.f, 46.f));
	line_3 = MeshBuilder::GenerateLine(Color(245.f, 76.f, 245.f));
	line_4 = MeshBuilder::GenerateLine(Color(223, 213, 224));
	line_axis = MeshBuilder::GenerateLine(Color(176.f, 85.f, 19.f), false);

	switchShapes = true;

	//Init Shapes--------------------------------------//
	Init_Shapes();
}

/********************************************************************************
Init shapes
********************************************************************************/
void Scene_SAT_Test::Init_Shapes()
{
	Shape::InitStatic();

	//Offseting-------------------------------------------------//
	dist = 260.f;

	//Projection points-------------------------------------------------//
	shapeProjPoints = new float*[20];
	for (int i = 0; i < 20; ++i)
		shapeProjPoints[i] = new float[20];

	shapeProjPoints_2ndCheck = new float*[20];
	for (int i = 0; i < 20; ++i)
		shapeProjPoints_2ndCheck[i] = new float[20];

	shapeProjPoints_2 = new float*[20];
	for (int i = 0; i < 20; ++i)
		shapeProjPoints_2[i] = new float[20];

	shapeProjPoints_2_2ndCheck = new float*[20];
	for (int i = 0; i < 20; ++i)
		shapeProjPoints_2_2ndCheck[i] = new float[20];

	testShape.Init("farkle", Vector3(0, 0, 0));

	//Test shape 1---------------------------------------//
	testShape.AddPoint(Vector3(-170.5f, -25.f, 0.f));
	testShape.AddPoint(Vector3(170.5f, -45.f, 0.f));
	testShape.AddPoint(Vector3(170.f, 35.f, 0.f));

	testShape.AddPoint(Vector3(-170.f, 25.f, 0.f));

	//calculate faces for this shape--------------------------//
	testShape.CalculateFaces();
	
	//Test shape 2---------------------------------------//
	testShape_2.Init("farkle2", Vector3(0, -70, 0));

	testShape_2.AddPoint(Vector3(-120.f, -50.f, 0.f));
	testShape_2.AddPoint(Vector3(120.f, -50.f, 0.f));
	testShape_2.AddPoint(Vector3(120.f, -10.f, 0.f));
	testShape_2.AddPoint(Vector3(20.f, 60.f, 0.f));
	testShape_2.AddPoint(Vector3(-100.f, 10.f, 0.f));

	//calculate faces for this shape--------------------------//
	testShape_2.CalculateFaces();
}

/********************************************************************************
Run
********************************************************************************/
void Scene_SAT_Test::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();

	//Stage 1: States, flags and values update ===========================================================//
	Update_Shapes();

	//Stage 2: TRS calculations for Entity and Comp ===========================================================//
	testShape.RecalculatePoints(true);	//called by calculate TRS with parents but for now stand-alone since no entity adds this
	testShape_2.RecalculatePoints(false);

	//stage 3: Update with changes ===========================================================//
	Calculate_ShapeProjections();

	//collision check----------------------------------------//
	testShape.CollisionCheck(testShape_2);

	//Stage 4: 2nd TRS calculations for Entity and Comp (For those with changes) ===========================================================//
	testShape.RecalculatePoints(false);	//called by calculate TRS with parents but for now stand-alone since no entity adds this
	testShape_2.RecalculatePoints(false);
}

/********************************************************************************
update shapes
********************************************************************************/
void Scene_SAT_Test::Update_Shapes()
{
	//change display state--------------------------------------//
	if (CU::input.IsKeyReleased(Input::O))
		switchShapes = !switchShapes;

	//translation----------------------------------------//
	Vector3 vel;
	if (CU::input.IsKeyPressed(Input::ARROW_UP))
		vel += Vector3(0, 2.f, 0);
	if (CU::input.IsKeyPressed(Input::ARROW_DOWN))
		vel += Vector3(0, -2.f, 0);
	if (CU::input.IsKeyPressed(Input::ARROW_LEFT))
		vel += Vector3(-2.f, 0, 0);
	if (CU::input.IsKeyPressed(Input::ARROW_RIGHT))
		vel += Vector3(2.f, 0, 0);
	testShape.Translate(vel);

	//rotation----------------------------------------//
	if (CU::input.IsKeyPressed(Input::K))
		testShape.Rotate(1.f);
	if (CU::input.IsKeyPressed(Input::L))
		testShape.Rotate(-1.f);
}

/********************************************************************************
cal shape projection
********************************************************************************/
void Scene_SAT_Test::Calculate_ShapeProjections()
{
	//Shape 1-----------------------------------------------------//
	testShape_2.ProjectShapeOntoThis(testShape, shapeProjPoints);	//project shape onto this

	//Shape 2-----------------------------------------------------//
	testShape_2.ProjectShapeOntoThis(testShape_2, shapeProjPoints_2);	//its own points must be projected too

	//Switch shapes-----------------------------------------------//
	testShape.ProjectShapeOntoThis(testShape_2, shapeProjPoints_2_2ndCheck);	
	testShape.ProjectShapeOntoThis(testShape, shapeProjPoints_2ndCheck);	//its own points must be projected too
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
	//projected axis-----------------------------------------//
	//if (switchShapes)	//shape 1's axes
	//	DrawShapeAxes(line_axis, testShape, dist);
	//else    //shape 2's axes
	//	DrawShapeAxes(line_axis, testShape_2, dist);

	//Draw shapes------------------------------------------------//
	Draw_Shapes();

	//Draw shapes projection------------------------------------------------//
	//Draw_ShapeProjection();

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
draw shape axes
********************************************************************************/
void Scene_SAT_Test::DrawShapeAxes(Mesh* line, Shape& shape, float offsetDist)
{
	Vector3 offset;
	for (int i = 0; i < shape.faceList.size(); ++i)
	{
		offset.Set(offsetDist * shape.faceList[i].dir.x, offsetDist * shape.faceList[i].dir.y, 0.f);
		DrawAxis(line, shape.faceList[i].normal, offset);
	}
}

/********************************************************************************
Wrappers
********************************************************************************/
void Scene_SAT_Test::Draw_Shapes()
{
	testShape.Draw();
	testShape_2.Draw();
}

void Scene_SAT_Test::Draw_ShapeProjection()
{
	//shape 1's projection
	if (switchShapes)
	{
		//lineMesh, projectedPoint_Mesh, projectee, projected, shapeProjPoints)
		Draw_ProjectedShape(line_4, line_4, sphere_4, testShape, testShape_2, shapeProjPoints_2_2ndCheck);
		Draw_ProjectedShape(line_2, line_2, sphere_2, testShape, testShape, shapeProjPoints_2ndCheck);
	}
	else
	{
		Draw_ProjectedShape(line_4, line_4, sphere_4, testShape_2, testShape, shapeProjPoints);
		Draw_ProjectedShape(line_2, line_2, sphere_2, testShape_2, testShape_2, shapeProjPoints_2);
	}
}

/********************************************************************************
Draw projected shape on another shapes's axes
********************************************************************************/
float min_ProjPt, max_ProjPt;	//min and max projected points
Vector3 min_pos, max_pos;
void Scene_SAT_Test::Draw_ProjectedShape(Mesh* lineMesh_min, Mesh* lineMesh_max, Mesh* projectedPoint_Mesh, Shape& projectee, Shape& projected, float** shapeProjPoints)
{
	Vector3 offset, projPos, axis;

	//Loop through all axis of projectee shape----------------------------------//
	for (int i = 0; i < projectee.faceList.size(); ++i)
	{
		//Set axis and offset---------------------------------------------------------------------//
		axis = projectee.faceList[i].normal;	//the axis
		CU::shared.CalculateOffset(offset, projectee.faceList[i].normal, dist);

		//Point 0---------------------------------------------------------------------//
		//projPos = Vector3(shapeProjPoints[i][0] * axis.x, shapeProjPoints[i][0] * axis.y, 0) + offset;
		min_pos = max_pos = projected.pointList[0].pos;
		min_ProjPt = max_ProjPt = shapeProjPoints[i][0];

		//loop through all points of projected, find the min and max point----------------------------------//
		for (int j = 1; j < projected.pointList.size(); ++j)
		{
			//calculate projected point--------------------------------------------//
			projPos = Vector3(shapeProjPoints[i][j] * axis.x, shapeProjPoints[i][j] * axis.y, 0) + offset;

			//cal min and max point------------------------------------//
			if (shapeProjPoints[i][j] < min_ProjPt)
			{
				min_ProjPt = shapeProjPoints[i][j];
				min_pos = projected.pointList[j].pos;
			}
			else if (shapeProjPoints[i][j] >= max_ProjPt)
			{
				max_ProjPt = shapeProjPoints[i][j];
				max_pos = projected.pointList[j].pos;
			}
		}

		//Draw min and max projected  points---------------------------------//
		Vector3 minProj = Vector3(min_ProjPt * axis.x, min_ProjPt * axis.y, 0) + offset;
		Vector3 maxProj = Vector3(max_ProjPt * axis.x, max_ProjPt * axis.y, 0) + offset;

		Draw_ProjectedPoints(lineMesh_min, projectedPoint_Mesh, min_pos, minProj, projectee.faceList[i].normal);
		Draw_ProjectedPoints(lineMesh_max, projectedPoint_Mesh, max_pos, maxProj, projectee.faceList[i].normal);
	}
}

/********************************************************************************
Drwa projected points
********************************************************************************/
void Scene_SAT_Test::Draw_ProjectedPoints(Mesh* lineMesh, Mesh* projectedPoint_Mesh, Vector3& pointPos, Vector3& projPos, Vector3 axisDir)
{
	//Draw the line-----------------------------------//
	Vector3 line = projPos - pointPos;
	Vector3 ptDir = line.Normalized();
	float angle = Vector3::getAngleFromDir(ptDir.x, ptDir.y);
	CU::shared.DrawLine(lineMesh, pointPos, angle, line.Length(), 1.5f);
}

/********************************************************************************
Drwa axis
********************************************************************************/
void Scene_SAT_Test::DrawAxis(Mesh* line, Vector3& axis, Vector3 offset)
{
	float angle = Vector3::getAngleFromDir(axis.x, axis.y);
	CU::shared.DrawLine_Offset(line, Vector3(0, 0, 0), angle, 1000.f, 3.f, offset);
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_SAT_Test::Exit()
{
	delete sphere;
	delete sphere_2;
	delete sphere_4;
	delete quad;
	delete axes;
	delete line_1;
	delete line_2;
	delete line_3;
	delete line_4;
	delete line_axis;

	for (int i = 0; i < 20; ++i)
		delete[] shapeProjPoints[i];
	delete[] shapeProjPoints;

	for (int i = 0; i < 20; ++i)
		delete[] shapeProjPoints_2ndCheck[i];
	delete[] shapeProjPoints_2ndCheck;

	for (int i = 0; i < 20; ++i)
		delete[] shapeProjPoints_2[i];
	delete[] shapeProjPoints_2;

	for (int i = 0; i < 20; ++i)
		delete[] shapeProjPoints_2_2ndCheck[i];
	delete[] shapeProjPoints_2_2ndCheck;

	//Call parent--------------------------------------//
	Scene::Exit();
}