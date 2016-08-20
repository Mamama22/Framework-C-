#include "EntityManager.h"

void EntityManager::Init()
{
	childrenList.resize(10);
	compList.resize(10);
}

/********************************************************************************
Register entity. Entity added to list and handle given.
********************************************************************************/
int EntityManager::RegisterEntity(Entity* regMe)
{
	int handle = entityList.size();
	regMe->Sethandle(handle);
	entityList.push_back(regMe);
	return handle;
}

/********************************************************************************
Get entity via handle
********************************************************************************/
Entity* EntityManager::GetEntity(int handle)
{
	return entityList[handle];
}

/********************************************************************************
Get the top parent of this entity by the handle
********************************************************************************/
Entity* EntityManager::GetTopParent_Entity(int handle)
{
	return entityList[handle]->GetTopParent();
}

/********************************************************************************
Get list of comp/child of entity with handle
********************************************************************************/
void EntityManager::GetEntityChildren(int handle)
{
	entityList[handle]->GetChildrenList(childrenList);
}

void EntityManager::GetEntityComp(int handle)
{
	entityList[handle]->GetCompList(compList);
}