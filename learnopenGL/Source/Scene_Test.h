#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher
Date: 1/7/2016
/*************************************************************/
class Scene_Test : public Scene
{
	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;

public:

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif