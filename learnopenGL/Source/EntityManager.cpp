#include "EntityManager.h"

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