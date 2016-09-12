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
void AABB::Init(const char* name, Mesh* debugMesh, Vector3 pos, Vector3 scale, bool affects_parent)
{
	active = true;
	this->affects_parent = affects_parent;
	Renderer::Init(name, debugMesh, pos, scale);
}

/********************************************************************************
Rotate by parent: AFFECTS TRS
-counter parent/ancestor's rotation
********************************************************************************/
void AABB::ByParent_Rotate(float angle, Vector3 axis)
{
	//add rotate angle to transform.angle first-------------------//
	transform.Rotate_byParent(angle, axis);

	//child TRS counters the parent rotation----------------//
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
Vector3 thisPos, checkPos;
bool AABB::CollisionCheck(AABB& checkMe)
{
	//Get the prev. collision check vel----------------------------//
	prevVel = vel;

	//get current pos----------------------------------------------------------------//
	thisPos = transform.pos + vel;
	checkPos = checkMe.transform.pos + checkMe.vel;

	//get info----------------------------------------------------------------------------------------------//
	float combinedX_length = transform.scale.x + checkMe.transform.scale.x;
	float combinedY_length = transform.scale.y + checkMe.transform.scale.y;

	//direction of offset-------------------------------------------------------------------------------------//
	float dirX = 1.f;
	float dirY = 1.f;
	if (thisPos.x < checkPos.x)
		dirX = -1.f;
	if (thisPos.y < checkPos.y)
		dirY = -1.f;

	//Get xtreme points X and Y axis--------------------------------------------------------------------------//
	xtremeX_points.x = (thisPos.x < checkPos.x) ? 
		thisPos.x - (transform.scale.x * 0.5f) : checkPos.x - (checkMe.transform.scale.x * 0.5f);
	xtremeX_points.y = (thisPos.x > checkPos.x) ? 
		thisPos.x + (transform.scale.x * 0.5f) : checkPos.x + (checkMe.transform.scale.x * 0.5f);

	xtremeY_points.x = (thisPos.y < checkPos.y) ?
		thisPos.y - (transform.scale.y * 0.5f) : checkPos.y - (checkMe.transform.scale.y * 0.5f);
	xtremeY_points.y = (thisPos.y > checkPos.y) ?
		thisPos.y + (transform.scale.y * 0.5f) : checkPos.y + (checkMe.transform.scale.y * 0.5f);

	//check if collide X or Y side--------------------------------------------------------------//
	float point_lengthX = xtremeX_points.y - xtremeX_points.x;
	float point_lengthY = xtremeY_points.y - xtremeY_points.x;

	// one/both sides not touching----------------------------------------------------------------------//
	if (point_lengthX > combinedX_length || point_lengthY > combinedY_length)
		return false;

	collided = true;


	//Offset vel away from box----------------------------------------------------------//
	float theOffset = 0.f;	//offset magnitude
	float offsetX = combinedX_length - point_lengthX;	
	float offsetY = combinedY_length - point_lengthY;

	//X side
	if (offsetX < offsetY)
	{
		//Collides on This's axis--------------------------------------------------------------------------------//
		theOffset = offsetX * dirX;
		AABB_dir.Set(1.f, 0.f);
	}
	else
	{
		//Collides on This's axis--------------------------------------------------------------------------------//
		theOffset = offsetY * dirY;
		AABB_dir.Set(0.f, 1.f);
	}

	//Get offset angle--------------------------------------------------------------//
	float transformAngle = CU::entityMan.GetEntity(entityHandle)->transform.angle;	//use local angle

	////if transformed by grandparent, use grandparent's angle, if not dir will be wrong------//
	if (transformBy_Ancestor_ID != -1)
		transformAngle = CU::entityMan.GetEntity(transformBy_Ancestor_ID)->transform.angle;
	
	//new vel----------------------------------------------------------//
	float angle = Vector3::getAngleFromDir(AABB_dir.x, AABB_dir.y);
	angle -= transformAngle;
	AABB_dir.x = cos(Math::DegreeToRadian(angle));
	AABB_dir.y = sin(Math::DegreeToRadian(angle));
	vel = AABB_dir * theOffset;

	//offset entity----------------------------------------------------//
	cout << "TA id: " << transformBy_Ancestor_ID << endl;
	if (transformBy_Ancestor_ID == -1)
		CU::entityMan.GetEntity(entityHandle)->Translate(vel * 1.01f);
	else
		CU::entityMan.GetEntity(transformBy_Ancestor_ID)->Translate(vel * 1.01f);
}

/********************************************************************************
Update
********************************************************************************/
void AABB::Update()
{
	//reset flags--------------------------------------------------------//

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
	mesh->Render(alpha);
}

/********************************************************************************
get/set
********************************************************************************/
bool AABB::CheckLineIntersection(const Vector3& lineOrigin, float lineAngle, Vector3& intersectedPos)
{
	return false;
}