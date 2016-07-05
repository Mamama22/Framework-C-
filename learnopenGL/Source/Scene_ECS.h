#ifndef SCENE_ECS_H
#define SCENE_ECS_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Scene_ECS : public Scene
{
	ostringstream ss;
	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;

	Entity testEnt;

	//Add comp----------------------//
	Vector3 startPos;
	int rendererCounter;
	int meshType;

	/******************** tmp Component system **********************/
	vector<Render_OnScreen> Render_OnScreen_List;

	/******************** Add component to test **********************/
	void AddRendererToTest();

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