#ifndef SCENE_PLANE_INTER_H
#define SCENE_PLANE_INTER_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher

Line to plane intersection test

Date: 16/8/2016
/*************************************************************/
class Scene_PlaneInter : public Scene
{
	/******************** var **********************/
	//ray----------------------//
	float rayAngle;
	float rayLength;
	Vector3 rayPos;
	Vector3 intersectedPos;


	//plane-------------------------//
	Vector3 planePos;
	float planeAngle;
	float planeLength;
	Vector3 planeNormal;

	//flags----------------------//
	bool intersectedPlane;

	/******************** Utility **********************/
	void RotateRay(float amt);
	void CheckIntersection();

public:

	Scene_PlaneInter();
	~Scene_PlaneInter();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif