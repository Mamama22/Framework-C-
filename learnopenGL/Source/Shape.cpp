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
	this->offset = this->pos;
	this->pos += shapePos;
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
void Face::Set(int startPoint_index, int endPoint_index, vector<Point>& pList, bool debug)
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

	////Draw the normal---------------------------------//
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
void Shape::Init(const char* name, Vector3 pos)
{
	active = true;
	Component::Init(name);
	transform.Set(pos, Vector3(1, 1, 1));
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
			face.Set(i, i + 1, pointList);
		else
			face.Set(i, 0, pointList);
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
	{
		list[i] = pointList[i].pos.Dot(dir);
	}
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
Rotate by parent: does not affect TRS
********************************************************************************/
void Shape::ByParent_Rotate(float angle, Vector3 axis)
{
	Component::ByParent_Rotate(angle, axis);

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
	//All points pos calculated with updated TRS------------------------------------------//
	for (int i = 0; i < pointList.size(); ++i)
	{
		CU::shared.mtx[2].SetToTranslation(pointList[i].offset.x, pointList[i].offset.y, 0.f);

		//final transformation matrix-----------------------------//
		CU::shared.mtx[3] = transform.finalTRS * CU::shared.mtx[2];

		pointList[i].pos.Set(1, 1, 0);
		pointList[i].pos = CU::shared.mtx[3] * pointList[i].pos;
	}

	transform.pos.Set(1, 1, 0);
	transform.pos = transform.finalTRS * transform.pos;

	/*for (int i = 0; i < pointList.size(); ++i)
	{
		if (i < pointList.size() - 1)
			faceList[i].Set(i, i + 1, pointList, transform.pos);
		else
			faceList[i].Set(i, 0, pointList, transform.pos);
	}*/
}

bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}
bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}


/********************************************************************************
Collision check
********************************************************************************/
bool collided1 = false, collided2 = false;
Vector3 normal1, normal2;
float bounce1 = 0.f, bounce2 = 0.f;
void Shape::CollisionCheck_2(Shape& obstacle)
{
	bounce1 = bounce2 = 100000000000000000000.f;
	float offsetDistSq_1 = 0.f;
	float offsetDistSq_2 = 0.f;

	//Collision check-------------------------------------//
	collided1 = SAT_CollisionCheck(obstacle, normal1, bounce1, true, offsetDistSq_1);	//obstacle onto THIS
	collided2 = obstacle.SAT_CollisionCheck(*this, normal2, bounce2, false, offsetDistSq_2);	//this onto OBSTACLE

	if (!collided1 || !collided2)
		return;
	
	bool go1 = false;	//use THIS

	if (bounce1 == bounce2)
	{
		if (offsetDistSq_1 > offsetDistSq_2)
		{
			go1 = true;
		}
	}
	else if (bounce1 < bounce2)
		go1 = true;

	//Collides on This's axis-------------------------------------//
	if (go1)
	{
		//Invert collided face's normal to face inwards which pushes itself away--------------------//
		normal1.x *= -1.f;
		normal1.y *= -1.f;

		//Offset direction by angle rotated---------------------------------------//
		float angle = Vector3::getAngleFromDir(normal1.x, normal1.y);
		//cout << "Angle: " << angle << endl;
		angle -= transform.angle;
		normal1.x = cos(Math::DegreeToRadian(angle));
		normal1.y = sin(Math::DegreeToRadian(angle));
		Vector3 offsetAway = normal1 * bounce1;
		Translate(offsetAway);
	}

	//Collides on Obstacle's axis-------------------------------------//
	else
	{
		//Offset direction by angle rotated---------------------------------------//
		float angle = Vector3::getAngleFromDir(normal2.x, normal2.y);

		angle -= transform.angle;
		normal2.x = cos(Math::DegreeToRadian(angle));
		normal2.y = sin(Math::DegreeToRadian(angle));
		
		Vector3 offsetAway = normal2 * bounce2;
		Translate(offsetAway);
	}

	//recalculate points--------------------------------//
	RecalculatePoints(false);
}

void Shape::TranslatePosWithAngle(Vector3& pos, Vector3 dir, float speed)
{
	float angle = Vector3::getAngleFromDir(dir.x, dir.y);
	angle -= transform.angle;
	dir.x = cos(Math::DegreeToRadian(angle));
	dir.y = sin(Math::DegreeToRadian(angle));

	pos += dir * speed;
}

/********************************************************************************
Project shape 'checkMe' onto this and check

returns: the projection in vec3 to 'push' shape out of collision
********************************************************************************/
float projectedPoints[2];	//this shape
float projectedPoints_other[2];	//check shape
vector<float> p_bounceList;

//check face with optimal projection----------------------------------------------------//
Vector3 projNormal;
Vector3 projPos;
Shape* THIS_SHAPE = NULL;
Shape* OTHER_SHAPE = NULL;
float furthestProjLen = 0.f;

float shortestLen = 0.f;
bool Shape::SAT_CollisionCheck(Shape& checkMe, Vector3& normal, float& bounce, bool thisShape, float& offsetDistSq)
{
	p_bounceList.clear();

	//loop through all faces----------------------------------------------------//
	for (int i = 0; i < faceList.size(); ++i)
	{
		float intersectedLen = 1000000.f;

		//project all points of CHECK onto normals of this face and get min/max------------------------------------------//
		ProjectOntoNormal(checkMe, faceList[i].normal, projectedPoints_other);

		//project all points of THIS onto normals of this face and get min/max------------------------------------------//
		ProjectOntoNormal(*this, faceList[i].normal, projectedPoints);

		//intersection test: if a axis is not intersected, no collision------------------------------------------------------------//
		if (IntersectionTest_2(projectedPoints, projectedPoints_other, intersectedLen))
		{
			if (bounce > intersectedLen)
			{
				bounce = intersectedLen;
			}
			p_bounceList.push_back(intersectedLen);
		}
		else
			return false;
	}

	//check face with optimal projection----------------------------------------------------//
	THIS_SHAPE = this;
	OTHER_SHAPE = &checkMe;
	furthestProjLen = 0.f;

	//if not THIS shape, swap------------------------------//
	if (!thisShape)
		swap(THIS_SHAPE, OTHER_SHAPE);
	

	//loop through shortest length/s, find the one that propells this shape furthest away----------------------------//
	int count = 0;
	for (int i = 0; i < faceList.size(); ++i)
	{
		//if is shortest length---------------------------------//
		if (p_bounceList[i] < bounce + 0.1f)
		{
			count++;
			projPos = THIS_SHAPE->transform.pos;
			projNormal = faceList[i].normal;

			//if is THIS shape, offset away from own normal------------------------------//
			if (thisShape)
			{
				projNormal.x *= -1.f;
				projNormal.y *= -1.f;
			}
			
			projPos += projNormal * p_bounceList[i];

			//the furthest away from pos is the correct projection------------------------------//
			if (furthestProjLen < abs((projPos - OTHER_SHAPE->transform.pos).LengthSquared()))
			{
				offsetDistSq = furthestProjLen = abs((projPos - OTHER_SHAPE->transform.pos).LengthSquared());
				normal = faceList[i].normal;
			}
		}
	}

	return true;
}

/********************************************************************************
Test intersection of projected pair 1 and 2
param intersectedLen: how much is intersected previously, if this check has a shorter
						intersection, than intersectedLen will be updated

return: True if new intersected length is assigned

proj_1: THIS shape
proj_2: CHECK shape

Note: Does not check if intersectedLen_assign smaller than new intersect
********************************************************************************/
bool Shape::IntersectionTest_2(float proj_1[], float proj_2[], float& intersectedLen_assign)
{
	//Total length if both min/max were lined together-------------------------------------//
	float totalLength = (proj_1[1] - proj_1[0]) + (proj_2[1] - proj_2[0]);

	//check overlap-------------------------------------//
	if (overlaps(proj_1[0], proj_1[1], proj_2[0], proj_2[1]))
	{
		//get the intersected length---------------------//
		float min = (proj_1[0] > proj_2[0]) ? proj_2[0] : proj_1[0];
		float max = (proj_1[1] > proj_2[1]) ? proj_1[1] : proj_2[1];
		float intersectedLen = totalLength - (max - min);


		//if is shortest---------------------//
		intersectedLen_assign = intersectedLen;		

		return true;
	}

	return false;
}


/********************************************************************************
Get projection
store[0] stores the min value
store[1] stores the max value

make sure projected shapes has at least 2 points
********************************************************************************/
void Shape::ProjectOntoNormal(Shape& projected, const Vector3& normal, float store[])
{
	float min = projected.pointList[0].pos.Dot(normal);
	float max = min;
	for (int j = 1; j < projected.pointList.size(); ++j)
	{
		float val = projected.pointList[j].pos.Dot(normal);	//project onto

		if (val < min)
			min = val;
		else if (val > max)
			max = val;
	}

	store[0] = min;
	store[1] = max;
}

/********************************************************************************
Cal TRS with parent
********************************************************************************/
void Shape::CalculateTRS_WithParent(const Mtx44& parentRotMat)
{
	Component::CalculateTRS_WithParent(parentRotMat);
	RecalculatePoints(false);
}

void Shape::Update()
{
	
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::Draw()
{
	if (!active)
		return;

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