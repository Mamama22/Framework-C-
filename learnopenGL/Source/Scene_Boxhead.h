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
	/******************** tmp Component system **********************/
	const static int TOTAL_RENDERER = 40;
	const static int TOTAL_AABB = 20;
	const static int TOTAL_ENTITY = 20;

	int rendererCounter;
	int colliderCounter;
	int entityCounter;

	//ostacles indexes (keep tract of shapes that are obstacle collider)------------------------//
	int obStart_index;
	int obEnd_index;

	//Player controlled entitites--------------------------------------------------------//
	int playerStart_index;
	int playerEnd_index;

	//Renderer comp assign----------------------//
	Renderer* Render_InWorld_List[TOTAL_RENDERER];

	//shape comp assign----------------------//
	AABB AABB_List[TOTAL_AABB];

	//Entity----------------------//
	Entity testEnt[TOTAL_ENTITY];

	//player entity---------------------------//
	Entity* player;
	Entity* pickUp;

	/******************** Utilities **********************/
	void AddRendererToEntity(Entity& addToMe, Mesh* mesh, Vector3 scale);
	void AddAsChild(Entity& parent, Entity& child);
	void Add_AABB(Entity& addToMe, Vector3 scale);

	/******************** Init **********************/
	void InitCharacter(Entity** turnMe, Vector3 pos);
	void InitObstacles();

	/******************** Update **********************/
	void UpdatePlayerInput();

public:

	Scene_Boxhead();
	~Scene_Boxhead();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif