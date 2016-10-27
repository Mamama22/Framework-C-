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
	regMe->SetHandle(handle);
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

/********************************************************************************
Pre-update
********************************************************************************/
void EntityManager::PreUpdate()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->PreUpdate();
	}
}

/********************************************************************************
Update stage 1
********************************************************************************/
void EntityManager::UpdateStage1()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->Update_Stage1();
	}
}

/********************************************************************************
update stage 2
********************************************************************************/
void EntityManager::UpdateStage2()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->Update_Stage2();
	}
}

/********************************************************************************
update stage 3
********************************************************************************/
void EntityManager::UpdateStage3()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->Update_Stage3();
	}
}

/********************************************************************************
update stage 4
********************************************************************************/
void EntityManager::UpdateStage4()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->Update_Stage4();
	}
}

/********************************************************************************
update TRS for all
********************************************************************************/
void EntityManager::Update_AllTRS()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i]->getActive())
			entityList[i]->CalculateTRS();
	}
}

/********************************************************************************
Exit
********************************************************************************/
void EntityManager::Exit()
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if (entityList[i])
			delete entityList[i];
	}
}