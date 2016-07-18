#include "Shape.h"
#include "CoreUtilities.h"
Vector3 Face::shareVec;
float** Shape::shapeProjPoints;	
float** Shape::shapeProjPoints_2ndCheck;
float** Shape::shapeProjPoints_2;	
float** Shape::shapeProjPoints_2_2ndCheck;
float** Shape::minMax_this;
float** Shape::minMax_projected;
float** Shape::minMax_this_2ndCheck;
float** Shape::minMax_projected_2ndCheck;

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
void Face::Set(int startPoint_index, int endPoint_index, vector<Point>& pList, Vector3 shapePos, bool debug)
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

void Shape::InitStatic()
{
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

	//
	minMax_this = new float*[20];
	for (int i = 0; i < 20; ++i)
		minMax_this[i] = new float[20];

	minMax_this_2ndCheck = new float*[20];
	for (int i = 0; i < 20; ++i)
		minMax_this_2ndCheck[i] = new float[20];

	minMax_projected = new float*[20];
	for (int i = 0; i < 20; ++i)
		minMax_projected[i] = new float[20];

	minMax_projected_2ndCheck = new float*[20];
	for (int i = 0; i < 20; ++i)
		minMax_projected_2ndCheck[i] = new float[20];
}

/********************************************************************************
Add point
********************************************************************************/
void Shape::Init(const char* name)
{
	Component::Init(name);
	transform.Set(Vector3(0, 0, 0), Vector3(1,1,1));
	prevPos.SetZero();
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
void Shape::GetProjections(Vector3& dir, float list[])
{
	for (int i = 0; i < pointList.size(); ++i)
		list[i] = pointList[i].pos.Dot(dir);
}

/********************************************************************************
project passed in shape onto this shape
Note: Pass in a array of Vec3 array with enough mem. allocated for total sides for this shape and
total points for projected shape
********************************************************************************/
void Shape::ProjectShapeOntoThis(Shape& projectMe, float** list)
{
	for (int i = 0; i < faceList.size(); ++i)
		projectMe.GetProjections(faceList[i].normal, list[i]);
}

/********************************************************************************
Translate: translate in direction of shape
********************************************************************************/
void Shape::Translate(Vector3 vel)
{
	Component::Translate(vel);
	this->vel = vel;	//if never translate, last vel before stationary is stored
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
void Shape::RecalculatePoints(bool debug)
{
	transform.pos.Set(1, 1, 1);
	transform.pos = transform.TRS * transform.pos;

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
collision check
********************************************************************************/
void Shape::CollisionCheck(Shape& obstacle)
{
	//this onto obstacle-----------------------------------------------------//
	obstacle.ProjectShapeOntoThis(*this, shapeProjPoints);	//project this onto obstacle
	obstacle.ProjectShapeOntoThis(obstacle, shapeProjPoints_2);	//its own points must be projected too

	//Switch shapes-----------------------------------------------//
	ProjectShapeOntoThis(obstacle, shapeProjPoints_2_2ndCheck);
	ProjectShapeOntoThis(*this, shapeProjPoints_2ndCheck);	//its own points must be projected too

	//Get the min and max points of both shapes on both axis--------------------------------//
	obstacle.GetMinMax(*this, shapeProjPoints, minMax_this);
	obstacle.GetMinMax(obstacle, shapeProjPoints_2, minMax_projected);

	//Switch shapes-----------------------------------------------//
	GetMinMax(obstacle, shapeProjPoints_2_2ndCheck, minMax_projected_2ndCheck);
	GetMinMax(*this, shapeProjPoints_2ndCheck, minMax_this_2ndCheck);

	//intersection test------------------------------------------//
	Vector3 dir1, dir2;
	float bounceVal_1 = 0.f;
	float bounceVal_2 = 0.f;
	bool collided_1 = false;
	bool collided_2 = false;

	collided_1 = obstacle.IntersectionTest(minMax_projected, minMax_this, dir1, bounceVal_1);
	collided_2 = IntersectionTest(minMax_this_2ndCheck, minMax_projected_2ndCheck, dir2, bounceVal_2);

	//Get vector-------------------------------------//
	vel = transform.pos - obstacle.transform.pos;

	//check collision-------------------------------------//
	if (collided_1 && collided_2)
	{
		//Use obstacle normal------------------------//
		if (bounceVal_1 < bounceVal_2)
		{
			if (dir1.x != 0.f)
			{
				if (vel.x < 0.f)
					dir1.x = -abs(dir1.x);
				else
					dir1.x = abs(dir1.x);
			}

			if (dir1.y != 0.f)
			{
				if (vel.y < 0.f)
				{
					dir1.y = -abs(dir1.y);
				}
					else
						dir1.y = abs(dir1.y);
			}

			//Offset direction by angle rotated---------------------------------------//
			float angle = Vector3::getAngleFromDir(dir1.x, dir1.y);
			angle -= transform.angle;
			dir1.x = cos(Math::DegreeToRadian(angle));
			dir1.y = sin(Math::DegreeToRadian(angle));

			Vector3 offsetAway = dir1 * abs(bounceVal_1);

			Translate(offsetAway);
		}

		//Use own normal------------------------//
		else
		{
			if (dir2.x != 0.f)
			{
				if (vel.x < 0.f)
					dir2.x = -abs(dir2.x);
				else
					dir2.x = abs(dir2.x);
			}

			if (dir2.y != 0.f)
			{
				if (vel.y < 0.f)
				{
					dir2.y = -abs(dir2.y);
				}
					else
						dir2.y = abs(dir2.y);
			}

			//Offset direction by angle rotated---------------------------------------//
			float angle = Vector3::getAngleFromDir(dir2.x, dir2.y);
			angle -= transform.angle;
			dir2.x = cos(Math::DegreeToRadian(angle));
			dir2.y = sin(Math::DegreeToRadian(angle));

			Vector3 offsetAway = dir2 * abs(bounceVal_2);

			Translate(offsetAway);
		}
	}
}

bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}
bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}

/********************************************************************************
Get min max points of shape onto THIS SHAPE'S AXIS
********************************************************************************/
bool Shape::IntersectionTest(float** minMax_This, float** minMaxProjected, Vector3& axisDir, float& bounceVal)
{
	float shortestLength = 100000000000000.f;

	//loop through this shapes axes------------------//
	for (int i = 0; i < faceList.size(); ++i)
	{
		//calculate total length-------------------------------------//
		float totalLength = (minMax_This[i][1] - minMax_This[i][0]) + (minMaxProjected[i][1] - minMaxProjected[i][0]);

		//check overlap-------------------------------------//
		if (!overlaps(minMax_This[i][0], minMax_This[i][1], minMaxProjected[i][0], minMaxProjected[i][1]))
		{
			return false;
		}

		//if overlap, check if shortest-------------------------------------//
		else
		{
			//get the intersected length---------------------//
			float min = (minMax_This[i][0] > minMaxProjected[i][0]) ? minMaxProjected[i][0] : minMax_This[i][0];
			float max = (minMax_This[i][1] > minMaxProjected[i][1]) ? minMax_This[i][1] : minMaxProjected[i][1];
			float intersectedLen = totalLength - (max - min);

			if (shortestLength > intersectedLen)
			{
				shortestLength = intersectedLen;
				axisDir = faceList[i].normal;
				bounceVal = intersectedLen;
			}
		}
	}
	return true;
}

/********************************************************************************
Get min max points of shape onto THIS SHAPE'S AXIS
********************************************************************************/
void Shape::GetMinMax(Shape& projected, float** projectedPoints, float** storeHere)
{
	float min_ProjPt, max_ProjPt;	//min and max projected points

	//Loop through all axis of projectee shape----------------------------------//
	for (int i = 0; i < faceList.size(); ++i)
	{
		//Point 0---------------------------------------------------------------------//
		min_ProjPt = max_ProjPt = projectedPoints[i][0];

		//loop through all points of projected, find the min and max point----------------------------------//
		for (int j = 1; j < projected.pointList.size(); ++j)
		{
			//cal min and max point------------------------------------//
			if (projectedPoints[i][j] < min_ProjPt)
				min_ProjPt = projectedPoints[i][j];

			else if (projectedPoints[i][j] >= max_ProjPt)
				max_ProjPt = projectedPoints[i][j];
		}

		//store the min max of projected shape onto THIS in array----------------------------//
		storeHere[i][0] = min_ProjPt;
		storeHere[i][1] = max_ProjPt;
	}
}

void Shape::Update()
{
	
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