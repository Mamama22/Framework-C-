#ifndef GRIDMAP_H
#define GRIDMAP_H

#include "Entity.h"


/*************************************************************
Gridmap

Author: Tan Yie Cher
Date: 24/8/2016
/*************************************************************/
class GridMap : public Entity
{


protected:

	/******************** Var **********************/

	/******************** Added/removed **********************/
	void Added(Entity* parent);	//if this entity added, do something
	void Removed();	//if this entity removed, do something

	//added/removed action----------------------------------//
	virtual void Added_ToEntity(Entity* addedTo);	//call for every entity in chain, immediate to bottom

	/******************** Derive urself functions **********************/
	virtual void UpdateEntity();

public:

	/******************** public var **********************/
	bool parentTransforming;
	Transformation transform;

	/******************** Constructor/destructor **********************/
	Entity();
	~Entity();

	/******************** Addind/removing child/comp, overload for customisation **********************/
	virtual void AddComponent(Component* comp);
	virtual void AddChildren(Entity* child);
	virtual void RemoveComponent(Component* comp);
	virtual void RemoveChildren(Entity* child);

	/******************** Core functions **********************/
	void Init(Vector3 pos, Vector3 scale);

	//Will be called everytime BEFORE comp updates-------------------//
	void PreUpdate();
	void Update();

	//Call in stage 2 and 4--------------------------------------//
	virtual void CalculateTRS();

	/******************** Transformation function **********************/
	void Translate(Vector3 vel);
	void Rotate(float angle, Vector3 axis);


	/******************** CALLED BY PARENT ONLY **********************/
	//TRS---------------------------------------------//
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat);

	//transformation---------------------------------------------//
	virtual void ByParent_Translate(Vector3 vel);
	virtual void ByParent_Rotate(float angle, Vector3 axis);

	/******************** Get set **********************/
	Entity* GetParent();
	Entity* GetTopParent();	//get the very top parent
	void GetChildrenList(vector<Entity*>& list);
	void GetCompList(vector<Component*>& list);
	int Gethandle();
	void Sethandle(int handle);

	/******************** abstract functions **********************/
	virtual void Exit(){}


	/********************************************************************************
	get the component (specify its type when calling this function)
	********************************************************************************/
	template<class T>
	T* GetComp(const char* name)
	{
		for (it = componentList.begin(); it != componentList.end() && strcmp((*it)->GetName(), name) != 0; ++it){}

		if (it == componentList.end())
			return NULL;	//throw wrongtype(); return NULL brings u to the location of the fucked up

		p = componentList[it - componentList.begin()];

#if ERROR_CHECK 1
		if (!dynamic_cast<T*>(p))
			throw WrongType();
#endif

		return static_cast<T*>(p);
	}
};

#endif