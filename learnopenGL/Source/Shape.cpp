#include "Shape.h"
#include "CoreUtilities.h"
Vector3 Face::shareVec;

/********************************************************************************
Constructor/dextructor
********************************************************************************/
Face::Face(){}
Face::Face(const Face& copy)
{
	this->start = copy.start;
	this->dir = copy.dir;
	this->len = copy.len;
	this->normal = copy.normal;
	this->angle = copy.angle;
	this->offset = copy.offset;
}
Face::~Face(){}

/********************************************************************************
Face init
********************************************************************************/
void Face::Set(Vector3 start, Vector3 end, Vector3 shapePos)
{
	this->start = start;
	this->dir = (end - start).Normalized();
	this->len = (end - start).Length();
	this->normal = this->dir.Cross(Vector3(0, 0, 1));
	this->angle = Vector3::getAngleFromDir(dir.x, dir.y);
	this->offset = this->start - shapePos;
}

/********************************************************************************
rotate by angle
angle, normal and direction changes
********************************************************************************/
void Face::Rotate(float angle)
{
	//angle-------------------------------------------------------//
	this->angle += angle;

	if (this->angle > 360.f)
		this->angle -= 360.f;
	else if (this->angle < 0.f)
		this->angle += 360.f;

	//dir-------------------------------------------------------//
	this->dir.x = cos(Math::DegreeToRadian(this->angle));
	this->dir.y = sin(Math::DegreeToRadian(this->angle));

	this->normal = this->dir.Cross(Vector3(0, 0, 1));
}

/********************************************************************************
Face draw
********************************************************************************/
void Face::Draw()
{
	//Draw the point---------------------------------//
	CU::sharedResources.DrawMesh(CU::sharedResources.sphere, this->start, 10.f);

	//Draw the dir---------------------------------//
	CU::sharedResources.DrawLine(CU::sharedResources.line_1, this->start, this->angle, this->len, 3.f);

	//Draw the normal---------------------------------//
	float normalAngle = Vector3::getAngleFromDir(normal.x, normal.y);
	shareVec = this->start;
	shareVec.x += len * 0.5f * dir.x;
	shareVec.y += len * 0.5f * dir.y;
	CU::sharedResources.DrawLine(CU::sharedResources.line_2, shareVec, normalAngle, 50.f, 1.f);
}

/********************************************************************************
Constructor/destructor
********************************************************************************/
Shape::Shape()
{
}

Shape::Shape(const Shape& copyMe) : Renderer(copyMe)
{
}

Shape::~Shape()
{
}

/********************************************************************************
Add point
********************************************************************************/
void Shape::Init(const char* name)
{
	Component::Init(name);
	transform.Set(Vector3(0, 0, 0), Vector3(1,1,1));
}

/********************************************************************************
Add point
********************************************************************************/
void Shape::AddFace(Vector3 start, Vector3 end)
{
	Face newFace;
	newFace.Set(start, end, transform.pos);
	faceList.push_back(newFace);
}

/********************************************************************************
Get projection (2 points facing the axis)
********************************************************************************/
void Shape::GetProjection(Vector3& dir, Line& projectedVec)
{

}

/********************************************************************************
Translate: translate in direction of shape
********************************************************************************/
void Shape::Translate(Vector3 vel)
{
	Component::Translate(vel);
	//
	//for (int i = 0; i < faceList.size(); ++i)
	//{
	//	CU::sharedResources.mtx[2].SetToTranslation(faceList[i].offset.x, faceList[i].offset.y, 0.f);

	//	//final transformation matrix-----------------------------//
	//	CU::sharedResources.mtx[3] = transform.TRS * CU::sharedResources.mtx[2];

	//	faceList[i].start.Set(1, 1, 1);
	//	faceList[i].start = CU::sharedResources.mtx[3] * faceList[i].start;
	//}
}

/********************************************************************************
Roatae: recalculate pos
********************************************************************************/
void Shape::Rotate(float angle)
{
	Component::Rotate(angle, Vector3(0,0,1));

	//rotate and change pos------------------------//
	for (int i = 0; i < faceList.size(); ++i)
	{
		//CU::sharedResources.mtx[2].SetToTranslation(faceList[i].offset.x, faceList[i].offset.y, 0.f);

		////final transformation matrix-----------------------------//
		//CU::sharedResources.mtx[3] = transform.TRS * CU::sharedResources.mtx[2];

		//faceList[i].start.Set(1, 1, 1);
		//faceList[i].start = CU::sharedResources.mtx[3] * faceList[i].start;

		//rotate--------------------------------------------//
		faceList[i].Rotate(angle);
	}
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::RecalculatePoints()
{
	for (int i = 0; i < faceList.size(); ++i)
	{
		CU::sharedResources.mtx[2].SetToTranslation(faceList[i].offset.x, faceList[i].offset.y, 0.f);

		//final transformation matrix-----------------------------//
		CU::sharedResources.mtx[3] = transform.TRS * CU::sharedResources.mtx[2];

		faceList[i].start.Set(1, 1, 1);
		faceList[i].start = CU::sharedResources.mtx[3] * faceList[i].start;
	}
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::Draw()
{
	Vector3 pointVec, dir;
	for (int i = 0; i < faceList.size(); ++i)
	{
		faceList[i].Draw();
	}
}