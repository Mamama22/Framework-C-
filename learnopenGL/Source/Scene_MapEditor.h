#ifndef SCENE_MAPEDITOR_H
#define SCENE_MAPEDITOR_H

#include "Scene.h"
#define AI_STEP_TIME 0.0001

/*************************************************************
Author: Tan Yie Cher

Map editor scene

Date: 1/10/2016
/*************************************************************/
class Scene_MapEditor : public Scene
{
	GridMap* gridmap;
	GridLayer* gridLayer;

	//editor
	int x_inter, y_inter;	//x/y intersect for tiles
	Vector3 inter_tilePos;
	float inter_tileScale;

	/******************** Init **********************/
	void InitGridmap();

	/******************** Update **********************/
	void UpdatePlayerInput();

public:

	Scene_MapEditor();
	~Scene_MapEditor();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();

	void Run_Stage1();
	void Run_Stage3();

	void DrawInWorld();
	void DrawOnScreen();
	void DrawGUI();
	void Exit();
};

#endif