#include "SharedResources.h"
#include "MeshBuilder.h"
#include "CoreUtilities.h"
#include "Load_TGA.h"

/********************************************************************************
Init
********************************************************************************/
void SharedResources::Init()
{
	quad = MeshBuilder::GenerateQuad(Color(0, 255, 0), 1.f, 1.f, false);
	quad_start0 = MeshBuilder::GenerateQuad(Color(52, 247, 111), 1.f, 1.f, true);
	quad_start0_1 = MeshBuilder::GenerateQuad(Color(152, 247, 111), 1.f, 1.f, true);
	quad_1 = MeshBuilder::GenerateQuad(Color(255, 255, 0), 1.f, 1.f, false);
	quad_2 = MeshBuilder::GenerateQuad(Color(0, 255, 255), 1.f, 1.f, false);
	boundingBox_Mesh = MeshBuilder::GenerateDebugQuad(Color(42, 212, 175));
	sphere = MeshBuilder::GenerateSphere(Color(255, 255, 0), 36, 36, 0.5f);
	sphere_1 = MeshBuilder::GenerateSphere(Color(26, 125, 44), 36, 36, 0.5f);
	line_1 = MeshBuilder::GenerateLine(Color(51.f, 211.f, 219.f), true);
	line_2 = MeshBuilder::GenerateLine(Color(248, 194, 252));
	line_3 = MeshBuilder::GenerateLine(Color(73.f, 196.f, 188.f));
	line_start0 = MeshBuilder::GenerateLine(Color(248, 194, 252), true);
	line_start0_1 = MeshBuilder::GenerateLine(Color(201, 124, 68), true);
	axes = MeshBuilder::GenerateAxes();

	//floor
	floorQuad = MeshBuilder::GenerateQuad(Color(104, 115, 88), 1.f, 1.f, false);
	playerQuad = MeshBuilder::GenerateQuad(Color(157, 212, 76), 1.f, 1.f, false);
	turretQuad = MeshBuilder::GenerateQuad(Color(28, 128, 65), 1.f, 1.f, false);
	debug_quad = MeshBuilder::GenerateDebugQuad(Color(219, 135, 39));
	debug_quad_start0 = MeshBuilder::GenerateDebugQuad(Color(219, 135, 39), true);

	//sprites types------------------------------------------------------------//
	explosion_sprite_type.Init(TEX_SPRITE_EXPLOSION, 6, 8);
	zombie_sprite_type.Init(TEX_SPRITE_ZOMBIE, 8, 8);

	//testures------------------------------------------------------------------//
	textureList[TEX_MUSIC] = LoadTGA("Image//game/CatPurple.tga");
	textureList[TEX_CURSOR] = LoadTGA("Image//game/middle_finger.tga");
	textureList[TEX_SPRITE_EXPLOSION] = LoadTGA("Image//game/explosion_hd.tga");
	textureList[TEX_SPRITE_ZOMBIE] = LoadTGA("Image//game/zombie_sprite_sheet.tga");
	textureList[TEX_PERRY] = LoadTGA("Image//game/perry_da_platypus.tga");
	textureList[TEX_MC_TILEMAP] = LoadTGA("Image//game/minecraft_tilemap.tga");
	textureList[TEX_AI_TILEMAP] = LoadTGA("Image//game/AI_tilemap.tga");

	//tilemap---------------------------------------------------------------//
	tilemapList[TILEMAP_MC].Init(2, 2, textureList[TEX_MC_TILEMAP]);
	tilemapList[TILEMAP_AI].Init(2, 2, textureList[TEX_AI_TILEMAP]);
	tilemapList[TILEMAP_PERRY].Init(1, 1, textureList[TEX_PERRY]);

	//preload some textures into meshes---------------------------------//
	playerQuad->SetTexture(TEX_MUSIC);
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
	delete line_3;
	delete axes;
	delete quad;
	delete quad_1;
	delete quad_2;
	delete debug_quad;
	delete debug_quad_start0;
	delete line_start0;
	delete line_start0_1;
	delete floorQuad;
	delete playerQuad;
	delete turretQuad;
	delete quad_start0;
	delete quad_start0_1;
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
***********************************************************************qwqw*********/
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
	CU::view.RenderMesh(*line, 1.f);
	glLineWidth(1.f);
}

void SharedResources::DrawLine(Mesh* line, Vector3 startPos, float angle, float length, float thickness)
{
	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.Translate(startPos.x, startPos.y, 0.f);
	CU::view.Rotate(angle, 0, 0, 1);
	CU::view.Scale(length, 1.f, 1.f);
	CU::view.RenderMesh(*line, 1.f);
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
	CU::view.RenderMesh(*mesh, 1.f);
}