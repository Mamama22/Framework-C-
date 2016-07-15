#include "Line.h"
#include "CoreUtilities.h"

/******************************************************************************
Set: the start and end(start + vector magnitude)
******************************************************************************/
void Line::Set(float startX, float startY, float endX, float endY)
{
	this->start.Set(startX, startY, 0.f);
	this->end = Vector3(endX, endY, 0);
	this->vecMag = this->end + this->start;
	dir = (this->end - this->start).Normalized();
	len = (this->end - this->start).Length();
}

/******************************************************************************
vector projection
dir vector must be normalized for maximum accuracy
******************************************************************************/
Vector3 Line::vectorProjection(Vector3& projected, Vector3& dir)
{
	Vector3 part1;
	part1.x = projected.Dot(dir);
	part1.y = dir.Dot(dir);

	Vector3 returnVec;
	returnVec.x = (part1.x * dir.x) / part1.y;
	returnVec.y = (part1.x * dir.y) / part1.y;

	return returnVec;
}

/******************************************************************************
Project this line's start and end onto projectOnto
******************************************************************************/
void Line::GetProjection(Vector3& dir, Line& projectedVec)
{
	//project start--------------------------------------//
	Vector3 proj_start = vectorProjection(this->start, dir);

	//project end---------------------------------------//
	Vector3 proj_end = vectorProjection(this->end, dir);

	projectedVec.Set(proj_start.x, proj_start.y, proj_end.x, proj_end.y);
}

/******************************************************************************
Draw this line
******************************************************************************/
void Line::Draw(Mesh* line, float thickness)
{
	Draw(line, thickness, Vector3(0, 0, 0));
}

/******************************************************************************
Draw this line offsetted by dist
******************************************************************************/
void Line::Draw(Mesh* line, float thickness, Vector3 offset)
{
	//Points---------------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(start.x + offset.x, start.y + offset.y, start.z);
	CU::view.Scale(10.f, 10.f, 1.f);
	CU::view.RenderMesh(*CU::sharedResources.sphere);

	CU::view.SetIdentity();
	CU::view.Translate(end.x + offset.x, end.y + offset.y, end.z);
	CU::view.Scale(10.f, 10.f, 1.f);
	CU::view.RenderMesh(*CU::sharedResources.sphere);

	//Line----------------------------------------------------------//
	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.Translate(start.x + offset.x, start.y + offset.y, 0.f);
	CU::view.Rotate(Vector3::getAngleFromDir(dir.x, dir.y), 0, 0, 1);
	CU::view.Scale(len, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);
}

/******************************************************************************
Draw right angled lines from both points
******************************************************************************/
void Line::DrawRightLines(Mesh* line, float thickness, Vector3 offset)
{
	Vector3 dir = this->dir.Cross(Vector3(0, 0, 1));

	//Points---------------------------------------------------------//
	CU::view.SetIdentity();
	CU::view.Translate(start.x + offset.x, start.y + offset.y, start.z);
	CU::view.Rotate(Vector3::getAngleFromDir(dir.x, dir.y), 0, 0, 1);
	CU::view.Scale(1200.f, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);

	CU::view.SetIdentity();
	CU::view.Translate(end.x + offset.x, end.y + offset.y, end.z);
	CU::view.Rotate(Vector3::getAngleFromDir(dir.x, dir.y), 0, 0, 1);
	CU::view.Scale(1200.f, 1.f, 1.f);
	CU::view.RenderMesh(*line);
	glLineWidth(1.f);
}