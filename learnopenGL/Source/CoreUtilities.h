#ifndef COREUTIL_H
#define COREUTIL_H
#include "View.h"
#include "Input.h"
#include "SharedResources.h"
#include "EntityManager.h"
#define TEST_COLLISION_X 250.f

/*************************************************************
Core utility class. Contains:
-View
-Input

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class CU
{
public:

	/******************** core utilities **********************/
	static View view;
	static Input input;
	static SharedResources shared;
	static EntityManager entityMan;
	static double dt;
	static float fps;

	/******************** wrapper functions **********************/
	static void Init()
	{
		input.Init();
		view.Init();
		shared.Init();
		entityMan.Init();
	}

	static void Exit()
	{
		view.Exit();
		shared.Exit();
	}
};

#endif