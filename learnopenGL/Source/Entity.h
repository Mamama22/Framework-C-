#ifndef ENTITY_H
#define ENTITY_H
#define ERROR_CHECK 0

//component types-----------------------------//
#include "Renderer.h"
#include "BoxCollision.h"

/********************************************************************************
error
********************************************************************************/
class WrongType : public std::exception
{
public:
	const char* what() { return "Wrong type"; }
};


/*************************************************************
Entity, work in progress

Contains components as well as children (entities)

Usage:
FOLLOW THE ORDER:
1) Add/Remove child/comp
2) Modify values, pos, scale, angle etc.
3) Calculate final TRS
4) update

Author: Tan Yie Cher
Date: 2/7/2016
/*************************************************************/
class Entity
{
	static vector<Component*>::iterator it;
	static Component* p;

	/******************** Var **********************/
	string name;

	/******************** List **********************/
	vector<Component*> componentList;
	vector<Entity*> children;
	
protected:

	/******************** Var **********************/
	Entity* parent;
	int handle;

	/******************** Added/removed **********************/
	void Added(Entity* parent);	//if this entity added, do something
	void Removed();	//if this entity removed, do something

	/******************** Derive urself functions **********************/
	virtual void UpdateEntity();

public:
	
	/******************** public var **********************/
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
	void Update();

	/******************** Transformation function **********************/
	void Translate(Vector3 vel);
	void Rotate(float angle, Vector3 axis);

	/******************** Entity Transformation function: ENTITY USE ONLY **********************/
	virtual void CalculateTRS();
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat);

	/******************** Get set **********************/
	Entity* GetParent();
	Entity* GetTopParent();	//get the very top parent
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