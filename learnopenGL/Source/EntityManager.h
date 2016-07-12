#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"

/*************************************************************
Entity manager. Only stores entities. DOES NOT UPDATE THEM

Type: manager class
Usage: Entity adds itself to this and gets a handle

Author: Tan Yie Cher
Date: 12/7/2016
/*************************************************************/
class EntityManager
{
protected:

	/******************** var **********************/
	vector<Entity*> entityList;

public:

	/******************** comp functions **********************/
	int RegisterEntity(Entity* regMe);

	/******************** get functions **********************/
	Entity* GetEntity(int handle);
	Entity* GetTopParent_Entity(int handle);
};


#endif