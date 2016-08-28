#ifndef COREUTIL_H
#define COREUTIL_H
#include "View.h"
#include "Input.h"
#include "SharedResources.h"
#include "EntityManager.h"
#include "RendererManager.h"
#include "SpriteAnimManager.h"
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
	static RendererManager rendererMan;
	static SpriteEAnimMan spriteMan;
	static double dt;
	static float fps;

	/******************** wrapper functions **********************/
	static void Init();
	static void Post_Init();	//call after scene's init

	/******************** Update function: Call all AFTER corrosponding scene functions **********************/
	static void PreUpdate();

	static void Update_Stage1();
	static void Update_Stage2();
	static void Update_Stage3();
	static void Update_Stage4();

	/******************** Render **********************/
	static void DrawOnScreen();

	/******************** Exit **********************/
	static void Exit();
};

#endif