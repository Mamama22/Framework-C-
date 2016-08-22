#ifndef SCENE_H
#define SCENE_H

#include "Header.h"
#include "CoreUtilities.h"
#include "FPS_Camera.h"
#include "MeshBuilder.h"
#include "Line.h"
#include "Shape.h"
#include "AABB.h"

//Entity types---------------------------//
#include "Entity.h"

/*************************************************************
Scene class, create your own scene:
1) Derive from this class and inherit all the necessary functions

Author: Tan Yie Cher
Date: 1/7/2016
/*************************************************************/
class Scene
{
public:
	/******************************************** Static resources ***************************************************/
	//camera--------------------------------//
	FPS_Cam camera;

protected:

	/******************************************** State ***************************************************/
	enum STATE
	{
		TRANSITIONING_IN,
		UPDATING,
		TRANSITIONING_OUT,
	};

	//Flags and state--------------------------------//
	STATE scene_state;

public:

	/******************** core functions: Extend the virtual ones!!! **********************/
	virtual void Init();
	virtual void Run();
	virtual void DrawInWorld();
	virtual void DrawOnScreen();
	virtual void Exit();

};

#endif