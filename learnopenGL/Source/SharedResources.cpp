#include "SharedResources.h"
#include "MeshBuilder.h"
#include "CoreUtilities.h"

/********************************************************************************
Init
********************************************************************************/
void SharedResources::Init()
{
	boundingBox_Mesh = MeshBuilder::GenerateDebugQuad(Color(42, 212, 175));
	sphere = MeshBuilder::GenerateSphere(Color(255, 255, 0), 36, 36, 0.5f);
	sphere_1 = MeshBuilder::GenerateSphere(Color(26, 125, 44), 36, 36, 0.5f);
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f));
	line_2 = MeshBuilder::GenerateLine(Color(248, 194, 252));
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
}

/********************************************************************************
Exit
********************************************************************************/
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