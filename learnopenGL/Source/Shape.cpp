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
void Face::Set(int startPoint_index, int endPoint_index, vector<Point>& pList)
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
plane intersection
********************************************************************************/
bool Face::intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t)
{
	float denom = n.Dot(l);

	if (denom > 1e-6)
	{
		Vector3 p0l0 = p0 - l0;
		t = p0l0.Dot(n) / denom;
		return (t >= 0);
	}

	return false;
}

/********************************************************************************
test line intersection
********************************************************************************/
bool Face::LineIntersection(const Vector3& lineOrigin, float lineAngle, vector<Point>& pList, Vector3& intersectedPos)
{
	//offset everything to (0,0)
	Vector3 offset = lineOrigin;

	Vector3 rayNormal;

	rayNormal.x = cos(Math::DegreeToRadian(lineAngle));
	rayNormal.y = sin(Math::DegreeToRadian(lineAngle));
	float t = 0.f;


	//plane pos
	Vector3 planePos = pList[start].pos + (dir * len * 0.5f);

	bool intersectedPlane = intersectPlane(-normal, planePos, lineOrigin, rayNormal, t);

	if (intersectedPlane)
	{
		intersectedPos = lineOrigin + rayNormal * t;

		//check if intersected pos is "out of range"
		float lensq = (intersectedPos - planePos).LengthSquared();
		float halfLenPlane = len * 0.5f;

		//out of range of the plane width
		if (lensq > halfLenPlane * halfLenPlane)
			intersectedPlane = false;

		//add with offset
		intersectedPos -= offset;
	}

	return intersectedPlane;
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
Shape::Shape() : Renderer()
{
	collided = false;
	parent_shape = NULL;
}

Shape::Shape(const Shape& copyMe) : Renderer(copyMe)
{
	collided = false;
	parent_shape = NULL;
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
}

/********************************************************************************
Roatae: recalculate pos
********************************************************************************/
void Shape::Rotate(float angle)
{
	Component::Rotate(angle, Vector3(0, 0, 1));

	//rotate and change pos------------------------//
	for (int i = 0; i < faceList.size(); ++i)
	{
		//rotate--------------------------------------------//
		faceList[i].Rotate(angle);
	}
}

/********************************************************************************
Transform by parent (Does not affect TRS)
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
If shae added to entity
********************************************************************************/
void Shape::Added_ToEntity(int handle)
{
	//Get list of children comp of grand parent
	CU::entityMan.GetEntityComp(handle);

	//Get their shape collider
	for (int j = 0; j < CU::entityMan.compList.size(); ++j)
	{
		if (CU::entityMan.CheckCompType<Shape>(CU::entityMan.compList[j]))	//if matchs shape
		{
			static_cast<Shape*>(CU::entityMan.compList[j])->childrenShapes.push_back(this);
			parent_shape = static_cast<Shape*>(CU::entityMan.compList[j]);
			break;
		}
	}
}

/********************************************************************************
Draw outlines
********************************************************************************/
void Shape::RecalculatePoints()
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
}

/********************************************************************************
Utilities
********************************************************************************/
bool isBetweenOrdered(float val, float lowerBound, float upperBound) {return lowerBound <= val && val <= upperBound;}
bool overlaps(float min1, float max1, float min2, float max2){
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);}

/********************************************************************************
Collision check
********************************************************************************/
bool collided1 = false, collided2 = false;
Vector3 normal1, normal2;
float bounce1 = 0.f, bounce2 = 0.f;
void Shape::CollisionCheck_2(Shape& obstacle)
{
	if (parent_shape && transformByGrandParent)
	{
		//	collide_withParent = !parent_shape->collided;	//cal. collision with Parent if Parent is not collided
	}
	
	//resetting---------------------------------------------------//
	bounce1 = bounce2 = 100000000000000000000.f;
	float offsetDistSq_1 = 0.f;
	float offsetDistSq_2 = 0.f;

	//Collision check--------------------------------------------------------------------------------------//
	collided1 = SAT_CollisionCheck(obstacle, normal1, bounce1, true, offsetDistSq_1);	//obstacle onto THIS
	collided2 = obstacle.SAT_CollisionCheck(*this, normal2, bounce2, false, offsetDistSq_2);	//this onto OBSTACLE

	//Absolute no collision--------------------------------------------------------------------------------//
	if (!collided1 || !collided2)
		return;

	collided = true;	//set the flag

	//Collision positive--------------------------------------------------------------------------------//
	bool go1 = false;	//use THIS SHAPE

	//same offset factor check------------------------//
	if (bounce1 == bounce2)
	{
		if (offsetDistSq_1 > offsetDistSq_2)
		{
			go1 = true;
		}
	}
	else if (bounce1 < bounce2)
		go1 = true;

	//Collides on This's axis--------------------------------------------------------------------------------//
	Vector3 offsetAway;
	//float transformAngle = transform.angle;	//use local angle

	//if transformed by grandparent, use grandparent's angle, if not dir will be wrong------//
	float transformAngle = CU::entityMan.GetTopParent_Entity(parentHandle)->transform.angle;


	//Collide offset--------------------------------------------------------------------------------------------//
	Vector3 theNormal = (go1) ? normal1 * -1.f : normal2;	//get the normal from collided side (if THIS shape normal, invert to face out)
	float theBounce = (go1) ? bounce1 : bounce2;

	//offset away------------------------------------------//
	float angle = Vector3::getAngleFromDir(theNormal.x, theNormal.y);
	angle -= transformAngle;
	theNormal.x = cos(Math::DegreeToRadian(angle));
	theNormal.y = sin(Math::DegreeToRadian(angle));
	offsetAway = theNormal * theBounce;

	//Set collision offset vel-------------------------------------------------------------------------------------//
	vel = offsetAway * 1.05f;

	CU::entityMan.GetTopParent_Entity(parentHandle)->Translate(vel);
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
		if (IntersectionTest(projectedPoints, projectedPoints_other, intersectedLen))
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
	for (int i = 0; i < faceList.size(); ++i)
	{
		//if is shortest length---------------------------------//
		if (p_bounceList[i] < bounce + 0.1f)
		{
			projPos = THIS_SHAPE->transform.pos;
			projNormal = faceList[i].normal;

			//if is THIS shape, offset away from own normal------------------------------//
			if (thisShape)
			{
				projNormal.x *= -1.f;
				projNormal.y *= -1.f;
			}

			//et the projected pos---------------------------------------------//
			projPos += projNormal * p_bounceList[i];

			//the furthest away from pos is the correct projection------------------------------//
			if (furthestProjLen < abs((projPos - OTHER_SHAPE->transform.pos).Length()))
			{
				offsetDistSq = furthestProjLen = abs((projPos - OTHER_SHAPE->transform.pos).Length());
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
bool Shape::IntersectionTest(float proj_1[], float proj_2[], float& intersectedLen_assign)
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
void Shape::CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform)
{
	Component::CalculateTRS_WithParent(parentRotMat, GrandParentTransform);

	//If first pass-----------------------------------------------------------//
	RecalculatePoints();
}

void Shape::PreUpdate()
{
	Component::PreUpdate();
	collided = false;
	vel.SetZero();
}

void Shape::Update()
{
	//reset flags--------------------------------------------------------//
	transformByGrandParent = false;
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

bool Shape::CheckLineIntersection(const Vector3& lineOrigin, float lineAngle, Vector3& intersectedPos)
{
	bool intersected = false;
	for (int i = 0; i < faceList.size(); ++i)
	{
		intersected = faceList[i].LineIntersection(lineOrigin, lineAngle, pointList, intersectedPos);

		//no concave shape, can do this
		if (intersected)
			return true;
	}
	return false;
}