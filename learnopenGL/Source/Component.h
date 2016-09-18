#ifndef COMP_H
#define COMP_H

#include "Header.h"
#include "Transformation.h"

/*************************************************************
Component, work in progress

Contains: 
-name for identification

Usage:
-Inherit to create your own component types
-All components MUST BE child of a entity

Rules:
-update called only when active == True

Author: Tan Yie Cher
Date: 2/7/2016
/*************************************************************/
class Component
{
protected:

	int transformBy_Ancestor_ID;	//is it transforming by ancestor?(parent is it's entity) If so, what's the ID of ELDEST transforming ancestor
	int entityHandle;
	const char* name;
	bool active;	//only updates when active

	/******************** optional abstract functions to overload **********************/
	virtual void UpdatingComp();

public:

	//Transform comp-------------------------//
	Transformation transform;

	Component();
	Component(const Component& copyMe);
	virtual ~Component();

	/******************** comp functions **********************/
	void Init(const char* name, Vector3 pos, Vector3 scale);
	virtual void PreUpdate();
	void Update();

	/******************** abstract functions **********************/
	virtual void Exit() = 0;

	/******************** Transformation function **********************/
	virtual void Translate(Vector3 vel);	//overload if applicable
	virtual void Rotate(float angle, Vector3 axis);

	/******************** CALLED BY PARENT ONLY **********************/
	//Add/removal---------------------------------------------//
	void Added(Transformation& parentTrans, int parentHandle);	//Init

	//Added to GRAND PARENT entity------------------------------//
	virtual void Added_ToGrandparent(int handle);	//Update, call for every entity in chain, immediate to bottom
	virtual void RemovedFrom_Grandparent(int handle);	//Update, call for every entity in chain, immediate to bottom
	
	//removal---------------------------------------------//
	void Removed();

	//transformation---------------------------------------------//
	virtual void ByParent_Translate(Vector3 vel);
	virtual void ByParent_Rotate(float angle, Vector3 axis);

	/******************** Entity Transformation function: CALLED BY ENTITY ONLY **********************/
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat, int transformBy_Ancestor_ID);

	/******************** Get functions **********************/
	const char* GetName();
	bool isActive();
	int GetTransByGrandParent();
	int GetParentHandle();

	/******************** State functions **********************/
	void SetActive(bool b);

};

#endif