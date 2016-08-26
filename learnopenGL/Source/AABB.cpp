#include "AABB.h"

#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
AABB::AABB() : Renderer()
{
	collided = false;
}

AABB::AABB(const AABB& copyMe) : Renderer(copyMe)
{
	collided = false;
}

AABB::~AABB()
{
}

/********************************************************************************
Add point
********************************************************************************/
void AABB::Init(const char* name, Mesh* debugMesh, Vector3 pos, Vector3 scale)
{
	active = true;
	Component::Init(name);
	transform.Set(pos, scale);
	this->mesh = debugMesh;
}

/********************************************************************************
Rotate by parent: AFFECTS TRS
-counter parent/ancestor's rotation
********************************************************************************/
void AABB::ByParent_Rotate(float angle, Vector3 axis)
{
	//rotate normally by parent first-------------------//
	Component::ByParent_Rotate(angle, axis);

	//child counters the parent rotation----------------//
	transform.Rotate(-angle, axis);
}

/********************************************************************************
Cal TRS with parent
********************************************************************************/
void AABB::CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform)
{
	Component::CalculateTRS_WithParent(parentRotMat, GrandParentTransform);
}

void AABB::PreUpdate()
{
	Component::PreUpdate();
	vel.SetZero();
}

/********************************************************************************
Collision check
********************************************************************************/
Vector2 xtremeX_points, xtremeY_points;
Vector3 thisBox_Pos, checkBox_Pos;	//[0] = left, [1] = right
Vector3 prevVel;
Vector3 AABB_dir;
bool AABB::CollisionCheck(AABB& checkMe)
{
	//Get the prev. collision check vel----------------------------//
	prevVel = vel;

	float combinedX_length = transform.scale.x + checkMe.transform.scale.x;
	float combinedY_length = transform.scale.y + checkMe.transform.scale.y;

	float dirX = 1.f;
	float dirY = 1.f;
	if (transform.pos.x < checkMe.transform.pos.x)
		dirX = -1.f;
	if (transform.pos.y < checkMe.transform.pos.y)
		dirY = -1.f;

	//Get xtreme points X and Y axis--------------------------------------------------------------------------//
	xtremeX_points.x = (transform.pos.x < checkMe.transform.pos.x) ? 
		transform.pos.x - (transform.scale.x * 0.5f) : checkMe.transform.pos.x - (checkMe.transform.scale.x * 0.5f);
	xtremeX_points.y = (transform.pos.x > checkMe.transform.pos.x) ? 
		transform.pos.x + (transform.scale.x * 0.5f) : checkMe.transform.pos.x + (checkMe.transform.scale.x * 0.5f);

	xtremeY_points.x = (transform.pos.y < checkMe.transform.pos.y) ?
		transform.pos.y - (transform.scale.y * 0.5f) : checkMe.transform.pos.y - (checkMe.transform.scale.y * 0.5f);
	xtremeY_points.y = (transform.pos.y > checkMe.transform.pos.y) ?
		transform.pos.y + (transform.scale.y * 0.5f) : checkMe.transform.pos.y + (checkMe.transform.scale.y * 0.5f);

	//check if collide X or Y side--------------------------------------------------------------//
	float point_lengthX = xtremeX_points.y - xtremeX_points.x;
	float point_lengthY = xtremeY_points.y - xtremeY_points.x;

	if (point_lengthX > combinedX_length || point_lengthY > combinedY_length)
		return false;

	collided = true;

	//collision true-----------------------------------------------------------------//
	vel.SetZero();	//reset offset vel

	//Get offset angle--------------------------------------------------------------//
	float transformAngle = CU::entityMan.GetEntity(parentHandle)->transform.angle;	//use local angle

	//if transformed by grandparent, use grandparent's angle, if not dir will be wrong------//
	if (transformByGrandParent)
		transformAngle = CU::entityMan.GetTopParent_Entity(parentHandle)->transform.angle;

	//the cos/sin valeus---------------------------------------------------//
	float sinAngle = sin(Math::DegreeToRadian(-transformAngle));
	float cosAngle = cos(Math::DegreeToRadian(-transformAngle));
	
	float offsetX = combinedX_length - point_lengthX;
	float offsetY = combinedY_length - point_lengthY;

	//minus off prev vel if have-------------------------------------------------//
	transform.pos -= prevVel;

	float theOffset = 0.f;	//offset magnitude

	//X side
	if (offsetX < offsetY)
	{
		//Collides on This's axis--------------------------------------------------------------------------------//
		theOffset = offsetX * dirX;

		//translate on the spot
		transform.pos.x += theOffset;
		AABB_dir.Set(1.f, 0.f);
	}
	else
	{
		//Collides on This's axis--------------------------------------------------------------------------------//
		theOffset = offsetY * dirY;

		//translate on the spot
		transform.pos.y += theOffset;
		AABB_dir.Set(0.f, 1.f);
	}

	//offset away------------------------------------------//
	float angle = Vector3::getAngleFromDir(AABB_dir.x, AABB_dir.y);
	angle -= transformAngle;
	AABB_dir.x = cos(Math::DegreeToRadian(angle));
	AABB_dir.y = sin(Math::DegreeToRadian(angle));
	vel = AABB_dir * theOffset;

	if (transformByGrandParent)	//ancestor transform with this shape
	{
		CU::entityMan.GetTopParent_Entity(parentHandle)->Translate(vel);
	}
	else
	{
		CU::entityMan.GetEntity(parentHandle)->Translate(vel);
	}
}

/********************************************************************************
Update
********************************************************************************/
void AABB::Update()
{
	//reset flags--------------------------------------------------------//
	transformByGrandParent = false;
}

/********************************************************************************
Draw outlines
********************************************************************************/
void AABB::Draw()
{
	if (!active)
		return;

	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	CU::view.RenderMesh(*mesh);
}

/********************************************************************************
get/set
********************************************************************************/
bool AABB::CheckLineIntersection(const Vector3& lineOrigin, float lineAngle, Vector3& intersectedPos)
{
	return false;
}