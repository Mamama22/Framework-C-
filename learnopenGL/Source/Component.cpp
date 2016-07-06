#include "Component.h"
Mtx44 Component::rotate;
Mtx44 Component::translate;
Mtx44 Component::translate2;
Mtx44 Component::TRS;

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
	transform.pos += vel;
}

/********************************************************************************
Rotate
********************************************************************************/
void Component::Rotate(float angle)
{
	transform.angle += angle;

	if (transform.angle < 0.f)
		transform.angle += 360.f;
	else if (transform.angle > 360.f)
		transform.angle -= 360.f;
}

/********************************************************************************
Rotate with entity (parent): when entity rotates, pos of this component
changes along the axis entity rotates
********************************************************************************/
void Component::RotateWithEntity(Vector3 new_ParentPos, Vector3 parentChildOffset, float angle)
{
	rotate.SetToIdentity();
	translate.SetToIdentity();	//for translation to entity pos
	translate2.SetToIdentity();	//for translatuon to new pos after following arc rotated by entity

	translate.SetToTranslation(new_ParentPos.x, new_ParentPos.y, new_ParentPos.z);
	translate2.SetToTranslation(parentChildOffset.x, parentChildOffset.y, parentChildOffset.z);
	rotate.SetToRotation(angle, 0, 0, 1);
	
	//Calculate new pos with TRS------------------------------------------------------//
	translate = translate * rotate * translate2;
	transform.pos.SetZero();
	transform.pos = translate * transform.pos;

	//rotate angle------------------------------------//
	Rotate(angle);
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