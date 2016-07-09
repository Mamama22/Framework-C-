#include "Entity.h"
Component* Entity::p;
vector<Component*>::iterator Entity::it;
Mtx44 Entity::sharedMtx[5];

/********************************************************************************
Constructor/destructor
********************************************************************************/
Entity::Entity(){ parent = NULL; }

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
	comp->Added(transform);
}

/********************************************************************************
Add child (Entity)
********************************************************************************/
void Entity::AddChildren(Entity* child)
{
	children.push_back(child);
	child->Added(this);

	//Do preparations for being a child-----------------------------------------//
	Entity* parent_ptr = this;

	//Get the highest parent------------------//
	vector<Entity*> parentList;
	while (parent_ptr)
	{
		parentList.push_back(parent_ptr);
		parent_ptr = parent_ptr->parent;
	}

	for (int i = parentList.size() - 1; i >= 0; --i)
		child->transform.AddedToParent(parentList[i]->transform);
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
			comp->Removed();
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
			child->Removed();
			children.pop_back();
			break;
		}
	}
}

/********************************************************************************
if this entity added/removed, do something
********************************************************************************/
void Entity::Added(Entity* parent)
{
	this->parent = parent;
	Entity* parent_ptr = this->parent;
}
void Entity::Removed()
{
	parent = NULL;
}

/********************************************************************************
Init func
********************************************************************************/
void Entity::Init(Vector3 pos)
{
	transform.Set(pos, Vector3(1, 1, 1));
}

/********************************************************************************
Translate entity and children + components
********************************************************************************/
void Entity::Translate(Vector3 vel)
{
	transform.Translate(vel);
}

/********************************************************************************
Rotates entity
********************************************************************************/
void Entity::Rotate(float angle, Vector3 axis)
{
	transform.Rotate(angle, axis);
}

/********************************************************************************
Calculate overall TRS as well as for children, 
********************************************************************************/
void Entity::CalculateTRS()
{
	if (parent)	//if have parent, TRS calculated before
		return;

	transform.finalTRS = transform.TRS;

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(transform.finalTRS);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->CalculateTRS_WithParent(transform.finalTRS);
}


/********************************************************************************
Rotate with entity (parent): when entity rotates, pos of this component
changes along the axis entity rotates
********************************************************************************/
void Entity::CalculateTRS_WithParent(Mtx44& parentRotMat)
{
	transform.finalTRS = parentRotMat * transform.TRS;

	for (int i = 0; i < children.size(); ++i)
		children[i]->CalculateTRS_WithParent(transform.finalTRS);
	for (int i = 0; i < componentList.size(); ++i)
		componentList[i]->CalculateTRS_WithParent(transform.finalTRS);
}

/********************************************************************************
Update if active
********************************************************************************/
void Entity::Update()
{
	//calculate TRS-----------------------------------------//
	CalculateTRS();

	UpdateEntity();
}


/********************************************************************************
Update entity
********************************************************************************/
void Entity::UpdateEntity()
{
}

/********************************************************************************
Get
********************************************************************************/
Entity* Entity::GetParent(){ return parent; }