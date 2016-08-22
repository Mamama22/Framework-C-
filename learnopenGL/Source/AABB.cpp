#include "AABB.h"

#include "CoreUtilities.h"

/********************************************************************************
Constructor/destructor
********************************************************************************/
AABB::AABB() : Renderer()
{
}

AABB::AABB(const AABB& copyMe) : Renderer(copyMe)
{
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
}

void AABB::Update()
{
	//reset flags--------------------------------------------------------//
	transformByGrandParent = false;
}

void AABB::CollisionCheck(AABB& checkMe)
{

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