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

Rules:
-update called only when active == True

Author: Tan Yie Cher
Date: 2/7/2016
/*************************************************************/
class Component
{
private:

	const char* name;
	bool active;	//only updates when active

protected:

	/******************** abstract functions **********************/
	virtual void UpdatingComp() = 0;

	//Static----------------------------------//
	static Mtx44 rotate, translate, translate2, TRS;

public:

	//Transform comp-------------------------//
	Transformation transform;

	Component();
	~Component();

	/******************** comp functions **********************/
	void Init(const char* name);
	void Update();

	/******************** abstract functions **********************/
	virtual void Exit() = 0;

	/******************** Transformation function **********************/
	virtual void Translate(Vector3 vel);	//overload if applicable
	virtual void Rotate(float angle);

	/******************** Entity Transformation function: ENTITY USE ONLY **********************/
	virtual void RotateWithEntity(Transformation& entTrans, float angle);

	/******************** Get functions **********************/
	const char* GetName();
	bool GetActive();

	/******************** State functions **********************/
	void SetActive(bool b);

};

#endif