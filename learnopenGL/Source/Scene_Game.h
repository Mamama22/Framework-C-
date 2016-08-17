#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher

Boxhead 2D game
Temporary entity manager

Date: 16/8/2016
/*************************************************************/
class Scene_Game : public Scene
{
	/******************** tmp Component system **********************/
	const static int TOTAL_RENDERER = 40;
	const static int TOTAL_SHAPE = 20;
	const static int TOTAL_ENTITY = 20;

	int rendererCounter;
	int colliderCounter;
	int entityCounter;

	int obStart_index;
	int obEnd_index;

	//Renderer comp assign----------------------//
	Render_InWorld Render_InWorld_List[TOTAL_RENDERER];

	//shape comp assign----------------------//
	Shape Shape_List[TOTAL_SHAPE];

	//Entity----------------------//
	Entity testEnt[TOTAL_ENTITY];

	//player entity---------------------------//
	Entity* player;
	float detectionRange;
	float rayLength;
	Vector3 gunIntersectedPos;
	bool intersectedPlane;

	/******************** Utilities **********************/
	void AddRendererToEntity(Entity& addToMe, Mesh* mesh, Vector3 scale);
	void AddAsChild(Entity& parent, Entity& child);
	void AddShape(Entity& addToMe);

	/******************** Init **********************/
	void InitPlayer();
	void InitObstacles();

	/******************** Update **********************/
	void UpdatePlayerInput();
	void UpdateRayGun();

	/******************** Render **********************/
	void RenderFloor();
	void RenderRay();

public:

	Scene_Game();
	~Scene_Game();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif