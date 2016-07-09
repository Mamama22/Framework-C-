#include "Component.h"
Mtx44 Component::sharedMtx[5];

Component::Component()
{
	active = false;
}

Component::Component(Component& copyMe)
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
Added
********************************************************************************/
void Component::Added(Transformation& parentTrans)
{
	transform.AddedToParent(parentTrans);

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
void Component::CalculateTRS_WithParent(Mtx44& parentRotMat)
{
	transform.finalTRS = parentRotMat * transform.TRS;
}

/********************************************************************************
Getter/setter
********************************************************************************/
const char* Component::GetName(){ return name; }
bool Component::GetActive(){ return active; }

/********************************************************************************
State
********************************************************************************/
void Component::SetActive(bool b){ active = b; }