#ifndef RENDERER_H
#define RENDERER_H

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

	bool transformByGrandParent;	//is it transforming by the grand-parent? Since component's parent is entity
	int parentHandle;
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
	void Init(const char* name);
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

	//Added to GRAND PARENT entity
	virtual void Added_ToEntity(int handle);	//Update, call for every entity in chain, immediate to bottom
	
	//removal---------------------------------------------//
	void Removed();

	//transformation---------------------------------------------//
	virtual void ByParent_Translate(Vector3 vel);
	virtual void ByParent_Rotate(float angle, Vector3 axis);

	/******************** Entity Transformation function: CALLED BY ENTITY ONLY **********************/
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform);

	/******************** Get functions **********************/
	const char* GetName();
	bool isActive();
	bool GetTransByGrandParent();

	/******************** State functions **********************/
	void SetActive(bool b);

};

#endif