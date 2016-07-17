#include "Shape.h"
#include "CoreUtilities.h"
Vector3 Face::shareVec;

/********************************************************************************
Constructor/dextructor for point
********************************************************************************/
Point::Point(){}
Point::Point(const Point& copy)
{
	this->pos = copy.pos;
	this->offset = copy.offset;
}
Point::~Point(){}

void Point::Set(Vector3 pos, Vector3 shapePos)
{
	this->pos = pos;
	this->offset = this->pos - shapePos;
}

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
}
Face::~Face(){}

/********************************************************************************
Face init
********************************************************************************/
void Face::Set(int startPoint_index, int endPoint_index, vector<Point>& pList, Vector3 shapePos)
{
	//Indexes--------------------------------------------------------//
	this->start = startPoint_index;
	this->end = endPoint_index;

	//
	this->dir = (pList[end].pos - pList[start].pos).Normalized();
	this->len = (pList[end].pos - pList[start].pos).Length();
	this->normal = this->dir.Cross(Vector3(0, 0, 1));
	this->angle = Vector3::getAngleFromDir(dir.x, dir.y);
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
void Face::Draw(vector<Point>& pointList)
{
	//Draw the point---------------------------------//
	CU::shared.DrawMesh(CU::shared.sphere, pointList[start].pos, 5.f);

	//Draw the dir---------------------------------//
	CU::shared.DrawLine(CU::shared.line_1, pointList[start].pos, this->angle, this->len, 3.f);

	//Draw the normal---------------------------------//
	//float normalAngle = Vector3::getAngleFromDir(normal.x, normal.y);
	//shareVec = pointList[start].pos;
	//shareVec.x += len * 0.5f * dir.x;
	//shareVec.y += len * 0.5f * dir.y;
	//CU::shared.DrawLine(CU::shared.line_2, shareVec, normalAngle, 50.f, 1.f);
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
void Shape::AddPoint(Vector3 pos)
{
	Point point;
	point.Set(pos, transform.pos);
	pointList.push_back(point);
}

/********************************************************************************
calculate faces after all points added
********************************************************************************/
void Shape::CalculateFaces()
{
	Face face;
	for (int i = 0; i < pointList.size(); ++i)
	{
		if (i < pointList.size() - 1)
			face.Set(i, i + 1, pointList, transform.pos);
		else
			face.Set(i, 0, pointList, transform.pos);
		faceList.push_back(face);
	}
}

/********************************************************************************
Get projection of all points
Note: Pass in a Vec3 array with enough mem. allocated for total points in this shape
********************************************************************************/
void Shape::GetProjections(Vector3& dir, Vector3 list[])
{
	for (int i = 0; i < pointList.size(); ++i)
		list[i] = CU::shared.vectorProjection(pointList[i].pos, dir);
}

/********************************************************************************
project passed in shape onto this shape
Note: Pass in a array of Vec3 array with enough mem. allocated for total sides for this shape and
total points for projected shape
********************************************************************************/
void Shape::ProjectShapeOntoThis(Shape& projectMe, Vector3** list)
{
	for (int i = 0; i < faceList.size(); ++i)
	{
		projectMe.GetProjections(faceList[i].normal, list[i]);
	}
}

/********************************************************************************
Translate: translate in direction of shape
********************************************************************************/
void Shape::Translate(Vector3 vel)
{
	Component::Translate(vel);
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
		//rotate--------------------------------------------//
		faceList[i].Rotate(angle);
	}
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::RecalculatePoints()
{
	for (int i = 0; i < pointList.size(); ++i)
	{
		CU::shared.mtx[2].SetToTranslation(pointList[i].offset.x, pointList[i].offset.y, 0.f);

		//final transformation matrix-----------------------------//
		CU::shared.mtx[3] = transform.TRS * CU::shared.mtx[2];

		pointList[i].pos.Set(1, 1, 1);
		pointList[i].pos = CU::shared.mtx[3] * pointList[i].pos;
	}
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::Draw()
{
	for (int i = 0; i < faceList.size(); ++i)
	{
		//point-----------------------------//
		faceList[i].Draw(pointList);
	}
}

/********************************************************************************
get/set
********************************************************************************/
int Shape::Get_TotalPoints(){ return pointList.size(); }