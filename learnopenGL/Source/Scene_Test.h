#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher

Test shape collision
Temporary collision manager

Date: 18/8/2016
/*************************************************************/
class Scene_Test : public Scene
{
	Entity* base;
	Entity* player;

	vector<AABB*> player_AABB;
	vector<AABB*> obstacle_AABB;

	/******************** Utilities **********************/
	void InitCharacter(Entity** pointer, Vector3 pos, Vector3 scale);
	static AABB* AddAABB(Entity* player, Vector3 pos, Vector3 scale, Mesh* boxMesh);
	static void AddAsChild(Entity& parent, Entity& child);
	static void RemoveChild(Entity& parent, Entity& child);


	/******************** Init **********************/

	/******************** Update **********************/
	void UpdatePlayerInput();

public:

	Scene_Test();
	~Scene_Test();

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