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

	int parentHandle;
	const char* name;
	bool active;	//only updates when active

	/******************** abstract functions **********************/
	virtual void UpdatingComp();

public:

	//Transform comp-------------------------//
	Transformation transform;

	Component();
	Component(const Component& copyMe);
	virtual ~Component();

	/******************** comp functions **********************/
	void Init(const char* name);
	void Update();

	/******************** abstract functions **********************/
	virtual void Exit() = 0;

	/******************** Transformation function **********************/
	virtual void Translate(Vector3 vel);	//overload if applicable
	virtual void Rotate(float angle, Vector3 axis);

	/******************** CALLED BY PARENT ONLY **********************/
	//Add/removal---------------------------------------------//
	void Added(Transformation& parentTrans, int parentHandle);
	void Removed();

	//transformation---------------------------------------------//
	virtual void ByParent_Rotate(float angle, Vector3 axis);

	/******************** Entity Transformation function: ENTITY USE ONLY **********************/
	virtual void CalculateTRS_WithParent(const Mtx44& parentRotMat);

	/******************** Get functions **********************/
	const char* GetName();
	bool isActive();

	/******************** State functions **********************/
	void SetActive(bool b);

};

#endif