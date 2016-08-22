#include "Component.h"
#include "CoreUtilities.h"

Component::Component()
{
	active = false;
	parentHandle = -1;
}

Component::Component(const Component& copyMe)
{
	active = copyMe.active;
}

Component::~Component()
{
}

/********************************************************************************
Init:
********************************************************************************/
void Component::Init(const char* name)
{
	this->name = name;
}

void Component::PreUpdate()
{
	transform.PreUpdate();
	transformByGrandParent = false;
}

/********************************************************************************
Update:
********************************************************************************/
void Component::Update()
{
	if (active)
		UpdatingComp();
}

/********************************************************************************
Comp update
********************************************************************************/
void Component::UpdatingComp()
{

}

/********************************************************************************
Translate
********************************************************************************/
void Component::Translate(Vector3 vel)	//overload if applicable
{
	transform.Translate(vel);
}

/********************************************************************************
Rotate
********************************************************************************/
void Component::Rotate(float angle, Vector3 axis)
{
	transform.Rotate(angle, axis);
}

/********************************************************************************
Rotate by parent: does not affect TRS
********************************************************************************/
void Component::ByParent_Rotate(float angle, Vector3 axis)
{
	transform.axis = axis;
	transform.angle += angle;
	if (transform.angle < 0.f)transform.angle += 360.f;
	else if (transform.angle > 360.f)transform.angle -= 360.f;
}

void Component::ByParent_Translate(Vector3 vel)
{

}

/********************************************************************************
Added
********************************************************************************/
void Component::Added(Transformation& parentTrans, int parentHandle)
{
	transform.AddedToParent(parentTrans);
	this->parentHandle = parentHandle;
}

/********************************************************************************
Added to entity action
********************************************************************************/
void Component::Added_ToEntity(int handle)
{
}

/********************************************************************************
Removed
********************************************************************************/
void Component::Removed()
{
	
}

/********************************************************************************
Rotate with entity (parent): when entity rotates, pos of this component
changes along the axis entity rotates

parentRotMat: parent's TRS
GrandParentTransform: transformed by parent, (Component's grandparent/ancestor)
********************************************************************************/
void Component::CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform)
{
	transform.Calculate_TRS_withParent(parentRotMat);

	//if entity's parent has transformation this frame------------------------//
	if (GrandParentTransform)
		transformByGrandParent = true;
}

/********************************************************************************
Getter/setter
********************************************************************************/
const char* Component::GetName(){ return name; }
bool Component::isActive(){ return active; }
bool Component::GetTransByGrandParent(){ return transformByGrandParent; }

/********************************************************************************
State
********************************************************************************/
void Component::SetActive(bool b){ active = b; }