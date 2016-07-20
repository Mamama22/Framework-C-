#include "SharedResources.h"
#include "MeshBuilder.h"
#include "CoreUtilities.h"

/********************************************************************************
Init
********************************************************************************/
void SharedResources::Init()
{
	quad = MeshBuilder::GenerateQuad(Color(0, 255, 0), 1.f, 1.f, false);
	boundingBox_Mesh = MeshBuilder::GenerateDebugQuad(Color(42, 212, 175));
	sphere = MeshBuilder::GenerateSphere(Color(255, 255, 0), 36, 36, 0.5f);
	sphere_1 = MeshBuilder::GenerateSphere(Color(26, 125, 44), 36, 36, 0.5f);
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(248, 194, 252));
	axes = MeshBuilder::GenerateAxes();
}

/********************************************************************************
Exit
********************************************************************************/
void SharedResources::Exit()
{
	delete boundingBox_Mesh;
	delete sphere;
	delete sphere_1;
	delete line_1;
	delete line_2;
	delete axes;
	delete quad;
}

/********************************************************************************
calculate offset (dir angle + 90)
********************************************************************************/
void SharedResources::CalculateOffset(Vector3& offset, Vector3& dir, float dist)
{
	float angle = Vector3::getAngleFromDir(dir.x, dir.y);
	angle += 90.f;

	float xDir = cos(Math::DegreeToRadian(angle));
	float yDir = sin(Math::DegreeToRadian(angle));

	offset.Set(xDir * dist, yDir * dist, 0.f);
}

/********************************************************************************
project vector onto axis of dir
********************************************************************************/
Vector3 SharedResources::vectorProjection(Vector3& projected, Vector3& dir)
{
	Vector3 part1;
	part1.x = projected.Dot(dir);
	part1.y = dir.Dot(dir);

	Vector3 returnVec;
	returnVec.x = (part1.x * dir.x) / part1.y;
	returnVec.y = (part1.x * dir.y) / part1.y;

	return returnVec;
}

/********************************************************************************
Exit
********************************************************************************/
void SharedResources::DrawLine_Offset(Mesh* line, Vector3 startPos, float angle, float length, float thickness, Vector3 offset)
{
	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.Translate(startPos.x + offset.x, startPos.y + offset.y, 0.f);
	CU::view.Rotate(angle, 0, 0, 1);
	CU::view.Scale(length, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);
}

void SharedResources::DrawLine(Mesh* line, Vector3 startPos, float angle, float length, float thickness)
{
	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.Translate(startPos.x, startPos.y, 0.f);
	CU::view.Rotate(angle, 0, 0, 1);
	CU::view.Scale(length, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);
}

/********************************************************************************
Draw mesh with uniform scale
********************************************************************************/
void SharedResources::DrawMesh(Mesh* mesh, Vector3 pos, float scale)
{
	CU::view.SetIdentity();
	CU::view.Translate(pos.x, pos.y, pos.z);
	CU::view.Scale(scale, scale, 1.f);
	CU::view.RenderMesh(*mesh);
}