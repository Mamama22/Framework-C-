#ifndef SCENE_BOXHEAD_H
#define SCENE_BOXHEAD_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher

Boxhead 2D game
Temporary entity manager

Date: 18/8/2016
/*************************************************************/
class Scene_Boxhead : public Scene
{
	Entity* player;
	Entity* pickup;
	Entity* pickup_1;
	AI_Map* AImap;

	GridMap* gridmap;
	vector<AABB*> player_AABB;
	vector<AABB*> obstacle_AABB;

	/******************** Utilities **********************/
	static AABB* InitCharacter(Entity** pointer, Vector3 pos, Vector3 box_scale, Mesh* boxMesh);
	static void AddAsChild(Entity& parent, Entity& child);
	static void RemoveChild(Entity& parent, Entity& child);


	/******************** Init **********************/
	void InitGridmap();

	/******************** Update **********************/
	void UpdatePlayerInput();

public:

	Scene_Boxhead();
	~Scene_Boxhead();

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