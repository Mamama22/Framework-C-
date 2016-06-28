#ifndef COREUTIL_H
#define COREUTIL_H
#include "View.h"
#include "Input.h"

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

	/******************** wrapper functions **********************/
	static void Init()
	{
		input.Init();
		view.Init();
	}

	static void Exit()
	{
		view.Exit();
	}
};

#endif