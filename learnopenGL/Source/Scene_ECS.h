#ifndef SCENE_ECS_H
#define SCENE_ECS_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Scene_ECS : public Scene
{
	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;

	Vector3 dir;
	Vector3 up;
	Vector3 right;

	Entity testEnt;
	Entity testEnt_1;
	Entity testEnt_2;
	bool added1, added2;	//added ent 1 and 2 as children alr?

	//Add comp----------------------//
	Vector3 offset, offset1, offset2;
	int rendererCounter;
	int meshType;

	/******************** tmp Component system **********************/
	const static int TOTAL_RENDERER = 40;
	Render_InWorld Render_InWorld_List[TOTAL_RENDERER];

	/******************** Add component to test **********************/
	void AddRendererToTest(Entity& addToMe, Vector3 offset, bool first);
	void AddTestEnt1();
	void AddTestEnt2();

public:

	Scene_ECS();
	~Scene_ECS();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif