#ifndef SCENE_INTERSCTION_H
#define SCENE_INTERSCTION_H

#include "Scene.h"

/*************************************************************
Author: Tan Yie Cher
Date: 24/7/2016
/*************************************************************/
class Scene_Intersection : public Scene
{
	/******************** var **********************/
	//ray----------------------//
	float rayAngle;
	float rayLength;
	Vector3 intersectedPos;

	//spheres----------------------//
	Vector3 spherePos;
	float sphereRadius;

	//flags----------------------//
	//0: no intersection
	//1: touches tip
	//2: intersects at 2 points
	int intersectedSphere;	

	/******************** Utility **********************/
	void RotateRay(float amt);
	void CheckIntersection();

public:

	Scene_Intersection();
	~Scene_Intersection();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif