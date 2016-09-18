#include "Component.h"
#include "CoreUtilities.h"

Component::Component()
{
	active = false;
	entityHandle = -1;
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
void Component::Init(const char* name, Vector3 pos, Vector3 scale)
{
	this->name = name;
	transform.Set(pos, scale);
}

void Component::PreUpdate()
{
	transform.PreUpdate();
	transformBy_Ancestor_ID = -1;
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
	transform.Rotate_byParent(angle, axis);
}

void Component::ByParent_Translate(Vector3 vel)
{
	transform.Translate_byParent(vel);
}

/********************************************************************************
Added
********************************************************************************/
void Component::Added(Transformation& parentTrans, int parentHandle)
{
	transform.AddedToParent(parentTrans);
	this->entityHandle = parentHandle;
}

/********************************************************************************
if current entity is added to parent entity
********************************************************************************/
void Component::Added_ToGrandparent(int handle)
{
}

/********************************************************************************
if current entity removed from parent entity
********************************************************************************/
void Component::RemovedFrom_Grandparent(int handle)
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
void Component::CalculateTRS_WithParent(const Mtx44& parentRotMat, int transformBy_Ancestor_ID)
{
	transform.Calculate_TRS_withParent(parentRotMat);

	//if entity's parent has transformation this frame------------------------//
	this->transformBy_Ancestor_ID = transformBy_Ancestor_ID;
}

/********************************************************************************
Getter/setter
********************************************************************************/
const char* Component::GetName(){ return name; }
bool Component::isActive(){ return active; }
int Component::GetTransByGrandParent(){ return transformBy_Ancestor_ID; }
int Component::GetParentHandle(){ return entityHandle; }

/********************************************************************************
State
********************************************************************************/
void Component::SetActive(bool b){ active = b; }
