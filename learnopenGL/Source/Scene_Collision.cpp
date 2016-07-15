#include "Scene_Collision.h"

Scene_Collision::Scene_Collision()
{
}


Scene_Collision::~Scene_Collision(){}

/********************************************************************************
Init
********************************************************************************/
void Scene_Collision::Init()
{
	//Call parent--------------------------------------//
	Scene::Init();

	//Meshes---------------------------------------//
	sphere = MeshBuilder::GenerateSphere(Color(0, 255, 0), 36, 36, 0.5f);
	quad = MeshBuilder::GenerateQuad(Color(0,255,0), 1.f, 1.f, false);
	axes = MeshBuilder::GenerateAxes();
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(56.f, 255.f, 46.f));
	line_3 = MeshBuilder::GenerateLine(Color(245.f, 76.f, 245.f));
	line_axis = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f), false);

	//Lines---------------------------------------//
	Init_ProjectedLines();
}

/********************************************************************************
Init projected lines
********************************************************************************/
void Scene_Collision::Init_ProjectedLines()
{
	//axis-------------------------------------------------//
	axisDir = Vector3(250.f, 100.f).Normalized();

	//projected line 1-------------------------------------------------//
	ori_projectedLine.Set(-250.f, -200.f, 100.f, -200.f);
	ori_projectedLine.GetProjection(axisDir, projectedLine);

	//projected line 2-------------------------------------------------//
	ori_projectedLine_2.Set(110.f, -200.f, 200.f, -200.f);
	ori_projectedLine_2.GetProjection(axisDir, projectedLine_2);

	//Offseting-------------------------------------------------//
	dist = 150.f;
	CalculateOffset(offset, axisDir, dist);
}

/********************************************************************************
calculate offset (dir angle + 90)
********************************************************************************/
void Scene_Collision::CalculateOffset(Vector3& offset, Vector3& dir, float dist)
{
	float angle = Vector3::getAngleFromDir(dir.x, dir.y);
	angle += 90.f;

	float xDir = cos(Math::DegreeToRadian(angle));
	float yDir = sin(Math::DegreeToRadian(angle));

	offset.Set(xDir * dist, yDir * dist, 0.f);
}

/********************************************************************************
Run
********************************************************************************/
void Scene_Collision::Run()
{
	//Call parent--------------------------------------//
	Scene::Run();
	
}

Vector3 Scene_Collision::vectorProjection(Vector3& projected, Vector3& projectOnto)
{
	Vector3 part1;
	part1.x = projected.Dot(projectOnto);
	part1.y = projectOnto.Dot(projectOnto);
	
	Vector3 returnVec;
	returnVec.x = (part1.x * projectOnto.x) / part1.y;
	returnVec.y = (part1.x * projectOnto.y) / part1.y;

	return returnVec;
}

/********************************************************************************
Draw in world
********************************************************************************/
void Scene_Collision::DrawInWorld()
{
	CU::view.UseShader(View::BASIC_SHADER);	//use basic shader
}

/********************************************************************************
Draw on screen
********************************************************************************/
void Scene_Collision::DrawOnScreen()
{
	//Axes----------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Scale(2000.f, 2000.f, 2000.f);
	CU::view.RenderMesh(*axes);

	//Projected lines---------------------------------------//
	Draw_ProjectedLines();

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
Draw line utility
********************************************************************************/
void Scene_Collision::DrawLine(Mesh* line, Vector3 startPos, Vector3 dir, float thickness, float len, Vector3 offset)
{
	startPos += offset;
	float angle = Vector3::getAngleFromDir(dir.x, dir.y);

	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.SetIdentity();
	CU::view.Translate(startPos.x, startPos.y, 0.f);
	CU::view.Rotate(Vector3::getAngleFromDir(dir.x, dir.y), 0, 0, 1);
	CU::view.Scale(len, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);
}

/********************************************************************************
Draw_ProjectedLines
********************************************************************************/
void Scene_Collision::Draw_ProjectedLines()
{
	DrawLine(line_axis, Vector3(0, 0, 0), axisDir, 3.f, 1000.f, offset);
	ori_projectedLine.Draw(line_2, 5.f);
	ori_projectedLine_2.Draw(line_2, 5.f);

	//projected line 1-------------------------------------//
	projectedLine.Draw(line_3, 5.f, offset);
	projectedLine.DrawRightLines(line_3, 2.f, offset);

	//projected line 2-------------------------------------//
	projectedLine_2.Draw(line_3, 5.f, offset);
	projectedLine_2.DrawRightLines(line_3, 2.f, offset);
}

/********************************************************************************
Exit
********************************************************************************/
void Scene_Collision::Exit()
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