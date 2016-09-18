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

	//to get list of child/comp from a entity (RESIZED TO 10)
	vector<Entity*> childrenList;
	vector<Component*> compList;

	void Init();

	/******************** comp functions **********************/
	int RegisterEntity(Entity* regMe);

	/******************** get functions **********************/
	Entity* GetEntity(int handle);
	Entity* GetTopParent_Entity(int handle);

	void GetEntityChildren(int handle);
	void GetEntityComp(int handle);

	/******************** Core functions **********************/
	void PreUpdate();
	void UpdateStage1();
	void UpdateStage2();
	void UpdateStage3();
	void UpdateStage4();

	/******************** Exit functions **********************/
	void Exit();

	/********************************************************************************
	Check if given entity is of this class type
	********************************************************************************/
	template<class T>
	bool CheckEntityType(int handle)
	{
		if (dynamic_cast<T*>(entityList[handle]))
			return true;

		return false;
	}

	/********************************************************************************
	Check if given component is of this class type
	********************************************************************************/
	template<class T>
	static bool CheckCompType(Component* checkMe)
	{
		if (dynamic_cast<T*>(checkMe))
				return true;

		return false;
	}
};


#endif