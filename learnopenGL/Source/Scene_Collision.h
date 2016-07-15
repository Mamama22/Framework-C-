#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "Scene.h"


/*************************************************************
Demo line projection

Author: Tan Yie Cher
Date: 13/7/2016
/*************************************************************/
class Scene_Collision : public Scene
{
	//Meshes-----------------------------------//
	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;
	Mesh* line_1;
	Mesh* line_2;
	Mesh* line_3;
	Mesh* line_axis;

	//axis and offset-----------------------------------//
	Vector3 axisDir;

	float dist;
	Vector3 offset;	//offset from origin
	
	//projected lines-------------------------------------//
	Line ori_projectedLine;
	Line projectedLine;

	Line ori_projectedLine_2;
	Line projectedLine_2;

	/******************** utility functions **********************/
	Vector3 vectorProjection(Vector3& projected, Vector3& projectOnto);
	void DrawLine(Mesh* line, Vector3 startPos, Vector3 dir, float thickness, float len, Vector3 offset);
	void CalculateOffset(Vector3& offset, Vector3& dir, float dist);

	/******************** wrapper functions **********************/
	void Init_ProjectedLines();
	void Draw_ProjectedLines();

public:

	Scene_Collision();
	~Scene_Collision();

	/******************** core functions: Extend the virtual ones!!! **********************/
	void Init();
	void Run();
	void DrawInWorld();
	void DrawOnScreen();
	void Exit();
};

#endif