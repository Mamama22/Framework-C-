#ifndef ENTITY_H
#define ENTITY_H
#define ERROR_CHECK 0

//component types-----------------------------//
#include "Renderer_GridMap.h"
#include "SP_Comp.h"
#include "AI_Comp.h"

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
transforming

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
	bool active;

	/******************** List **********************/
	vector<Component*> componentList;
	vector<Entity*> children;
	int transformBy_Ancestor_ID;	//is it transforming by ancestor? If so, what's the ID of ELDEST transforming ancestor (Use to modify ancestor)
	
protected:

	/******************** Var **********************/
	Entity* parent;
	int handle;
	
	/******************** Added/removed **********************/
	void Added(Entity* parent);	//if this entity added, do something
	void Removed();	//if this entity removed, do something


public:
	
	/**************************************** public var ******************************************/
	bool transforming;	//is it transforming by itself?
	Transformation transform;
	bool sticky;	//transform affects parent

	/**************************************** Constructor/destructor ******************************************/
	Entity();
	virtual ~Entity();
	
	/**************************************** Addind/removing child/comp, overload for customisation ******************************************/
	virtual void AddComponent(Component* comp);
	virtual void AddChildren(Entity* child);
	virtual void RemoveComponent(Component* comp);
	virtual void RemoveChildren(Entity* child);

	/**************************************** Core functions ******************************************/
	void Init(Vector3 pos, Vector3 scale);

	//Will be called everytime BEFORE comp updates-------------------//
	virtual void PreUpdate();
	virtual void Update_Stage1();
	virtual void Update_Stage2();
	virtual void Update_Stage3();
	virtual void Update_Stage4();

	//Call in stage 2 and 4--------------------------------------//
	virtual void CalculateTRS();

	/**************************************** Transformation function ******************************************/
	virtual void Translate(Vector3 vel);
	virtual void Relocate(Vector3 pos);
	virtual void Rotate(float angle, Vector3 axis);


	/**************************************** CALLED BY PARENT ONLY ******************************************/
	//TRS---------------------------------------------//
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat, int transformBy_Ancestor_ID);

	//transformation---------------------------------------------//
	virtual void ByParent_Translate(Vector3 vel);
	virtual void ByParent_Rotate(float angle, Vector3 axis);

	/**************************************** Get set ******************************************/
	Entity* GetParent();
	Entity* GetTopParent();	//get the very top parent
	void GetChildrenList(vector<Entity*>& list);
	void GetCompList(vector<Component*>& list);
	int GetHandle();
	void SetHandle(int handle);
	bool getActive();
	void SetActive(bool b);

	/**************************************** abstract functions ******************************************/
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