#include "Entity.h"
Component* Entity::p;
vector<Component*>::iterator Entity::it;

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