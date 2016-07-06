#include "Entity.h"
Component* Entity::p;
vector<Component*>::iterator Entity::it;
Mtx44 Entity::rotate;
Mtx44 Entity::translate;
Mtx44 Entity::translate2;
Mtx44 Entity::TRS;

/********************************************************************************
Constructor/destructor
********************************************************************************/
Entity::Entity(){}

Entity::~Entity()
{
	componentList.clear(); 
	children.clear(); 
}

/********************************************************************************
Add component
********************************************************************************/
void Entity::AddComponent(Component* comp)
{
	componentList.push_back(comp);
}

/********************************************************************************
Add child (Entity)
********************************************************************************/
void Entity::AddChildren(Entity* child)
{
	children.push_back(child);
}

/********************************************************************************
Remove component
********************************************************************************/
void Entity::RemoveComponent(Component* comp)
{
	for (int i = 0; i < componentList.size(); ++i)
	{
		if (comp == componentList[i])
		{
			componentList[i] = componentList.back();
			componentList.pop_back();
			break;
		}
	}
}

/********************************************************************************
Remove child (Entity)
********************************************************************************/
void Entity::RemoveChildren(Entity* child)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (child == children[i])
		{
			children[i] = children.back();
			children.pop_back();
			break;
		}
	}
}

/********************************************************************************
Init func
********************************************************************************/
void Entity::Init(Vector3 pos)
{
	transform.pos = pos;
	transform.scale = Vector3(1, 1, 1);	//default
}

/********************************************************************************
Translate entity and children + components
********************************************************************************/
void Entity::Translate(Vector3 vel)
{
	transform.pos += vel;

	for (int i = 0; i < children.size(); ++i)
		children[i]->Translate(vel);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->Translate(vel);
}

/********************************************************************************
Rotates entity and children + components
********************************************************************************/
void Entity::Rotate(float angle)
{
	transform.angle += angle;
	
	if (transform.angle < 0.f)
		transform.angle += 360.f;
	else if (transform.angle > 360.f)
		transform.angle -= 360.f;

	//Children/comp must move along arc of rotation-------------------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->RotateWithEntity(transform.pos, children[i]->transform.pos - transform.pos, angle);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->RotateWithEntity(transform.pos, componentList[i]->transform.pos - transform.pos, angle);
}

/********************************************************************************
Rotate with entity (parent): when entity rotates, pos of this component
changes along the axis entity rotates
********************************************************************************/
void Entity::RotateWithEntity(Vector3 new_ParentPos, Vector3 parentChildOffset, float angle)
{
	Vector3 originalPos = transform.pos;

	rotate.SetToIdentity();

	//Add angle------------------------------------//
	transform.angle += angle;

	if (transform.angle < 0.f)
		transform.angle += 360.f;
	else if (transform.angle > 360.f)
		transform.angle -= 360.f;

	//Matrices operation-------------------------------------------------------------------//
	translate.SetToTranslation(new_ParentPos.x, new_ParentPos.y, new_ParentPos.z);
	translate2.SetToTranslation(parentChildOffset.x, parentChildOffset.y, parentChildOffset.z);
	rotate.SetToRotation(angle, 0, 0, 1);

	//Calculate new pos with TRS------------------------------------------------------//
	translate = translate * rotate * translate2;
	transform.pos.SetZero();
	transform.pos = translate * transform.pos;

	//Children/comp must move along arc of rotation-------------------------//
	for (int i = 0; i < children.size(); ++i)
		children[i]->RotateWithEntity(transform.pos, children[i]->transform.pos - originalPos, angle);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->RotateWithEntity(transform.pos, componentList[i]->transform.pos - originalPos, angle);
}

/********************************************************************************
Update if active
********************************************************************************/
void Entity::Update()
{
	//velocity based update---------------------------------//

	//assign prev pos---------------------------------------//
	transform.prevPos = transform.pos;
}