#include "Component.h"
Mtx44 Component::rotate;
Mtx44 Component::translate;
Mtx44 Component::translate2;
Mtx44 Component::TRS;

Component::Component()
{
	active = false;
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
void Component::RotateWithEntity(Transformation& entTrans, float angle)
{
	rotate.SetToIdentity();
	translate.SetToIdentity();	//for translation to entity pos
	translate2.SetToIdentity();	//for translatuon to new pos after following arc rotated by entity

	translate.SetToTranslation(entTrans.pos.x, entTrans.pos.y, entTrans.pos.z);
	Vector3 distFromEnt = transform.pos - entTrans.pos;	//dist from entity to comp
	translate2.SetToTranslation(distFromEnt.x, distFromEnt.y, distFromEnt.z);
	rotate.SetToRotation(angle, 0, 0, 1);
	
	//Calculate new pos with TRS------------------------------------------------------//
	translate = translate * rotate * translate2;
	transform.pos.SetZero();
	transform.pos = translate * transform.pos;
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