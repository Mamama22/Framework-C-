#include "Component.h"
#include "CoreUtilities.h"

Component::Component()
{
	active = false;
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

/********************************************************************************
Added
********************************************************************************/
void Component::Added(Transformation& parentTrans, int parentHandle)
{
	transform.AddedToParent(parentTrans);
	this->parentHandle = parentHandle;

	//offset angle-------------------------------//
	Entity* bottomParent = CU::entityMan.GetEntity(parentHandle);

	while (bottomParent)
	{
		ByParent_Rotate(-bottomParent->transform.angle, Vector3(0, 0, 1));
		bottomParent = bottomParent->GetParent();
	}
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
********************************************************************************/
void Component::CalculateTRS_WithParent(const Mtx44& parentRotMat)
{
	transform.Calculate_TRS_withParent(parentRotMat);
}

/********************************************************************************
Getter/setter
********************************************************************************/
const char* Component::GetName(){ return name; }
bool Component::isActive(){ return active; }

/********************************************************************************
State
********************************************************************************/
void Component::SetActive(bool b){ active = b; }