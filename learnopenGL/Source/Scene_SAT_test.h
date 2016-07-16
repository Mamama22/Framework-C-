#ifndef SCENE_SAT_TEST_H
#define SCENE_SAT_TEST_H

#include "Scene.h"


/*************************************************************
Demo line projection

Author: Tan Yie Cher
Date: 13/7/2016
/*************************************************************/
class Scene_SAT_Test : public Scene
{
	//Meshes-----------------------------------//
	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;
	Mesh* line_1;
	Mesh* line_2;
	Mesh* line_3;
	Mesh* line_axis;

	//Shape--------------------------------------------//
	Shape testShape;

	/******************** wrapper functions **********************/
	void Init_Shapes();
	void Update_Shapes();
	void Draw_Shapes();

public:

	Scene_SAT_Test();
	~Scene_SAT_Test();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif