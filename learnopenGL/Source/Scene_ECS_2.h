#ifndef SCENE_ECS_2_H
#define SCENE_ECS_2_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Scene_ECS_2 : public Scene
{
	/******************** tmp Component system **********************/
	const static int TOTAL_RENDERER = 40;
	const static int TOTAL_SHAPE = 20;
	const static int TOTAL_ENTITY = 20;

	int rendererCounter;
	int colliderCounter;
	int entityCounter;

	//Renderer comp assign----------------------//
	Render_InWorld Render_InWorld_List[TOTAL_RENDERER];

	//shape comp assign----------------------//
	Shape Shape_List[TOTAL_SHAPE];

	//Entity----------------------//
	Entity testEnt[TOTAL_ENTITY];

	/******************** Utilities **********************/
	void AddRendererToTest(Entity& addToMe, Mesh* mesh, Vector3 pos, Vector3 scale);
	void AddAsChild(Entity& parent, Entity& child);
	void AddShape(Entity& addToMe);

public:

	Scene_ECS_2();
	~Scene_ECS_2();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif