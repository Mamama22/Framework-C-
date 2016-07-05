#ifndef ENTITY_H
#define ENTITY_H
#define ERROR_CHECK 0

//component types-----------------------------//
#include "Renderer.h"

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

	//Static----------------------------------//
	static Mtx44 rotate, translate, translate2, TRS;

public:
	
	/******************** Addind/removing child/comp, overload for customisation **********************/
	virtual void AddComponent(Component* comp);
	virtual void AddChildren(Entity* child);
	virtual void RemoveComponent(Component* comp);
	virtual void RemoveChildren(Entity* child);

	//Transform comp-------------------------//
	Transformation transform;

	void Init(Vector3 pos);

	/******************** abstract functions **********************/

	//Will be called everytime BEFORE comp updates-------------------//
	virtual void Update(){}

	virtual void Exit(){}

	/******************** Transformation function **********************/
	virtual void Translate(Vector3 vel);	//overload if applicable
	virtual void Rotate(float angle);

	/******************** Entity Transformation function: ENTITY USE ONLY **********************/
	void RotateWithEntity(Vector3 new_ParentPos, Vector3 parentChildOffset, float angle);	//call rotate with parent

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