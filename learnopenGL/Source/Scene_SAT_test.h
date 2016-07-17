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
	Mesh* sphere_2;
	Mesh* sphere_4;
	Mesh* line_1;
	Mesh* line_2;
	Mesh* line_3;
	Mesh* line_4;
	Mesh* line_axis;

	//Shape--------------------------------------------//
	Shape testShape;
	Shape testShape_2;
	bool switchShapes;	//switch to display another shape projection

	//Offset------------------------------------------//
	float dist;

	//shape 1----------------------------//
	float** shapeProjPoints;	//to store shape projected points
	float** shapeProjPoints_2ndCheck;	//to store shape projected points for checking another shape

	//shape 2----------------------------//
	float** shapeProjPoints_2;	//to store shape projected points
	float** shapeProjPoints_2_2ndCheck; //to store shape projected points for checking another shape

	/******************** wrapper functions **********************/
	void Init_Shapes();
	
	void Update_Shapes();
	void Calculate_ShapeProjections();

	void Draw_Shapes();
	void Draw_ShapeProjection();

	/******************** Uti functions **********************/
	void DrawShapeAxes(Mesh* line, Shape& shape, float offsetDist);
	void DrawAxis(Mesh* line, Vector3& axis, Vector3 offset);

	void Draw_ProjectedShape(Mesh* lineMesh, Mesh* projectedPoint_Mesh, Shape& projectee, Shape& projected, float** shapeProjPoints);
	void Draw_ProjectedPoints(Mesh* lineMesh, Mesh* projectedPoint_Mesh, Vector3& pointPos, Vector3& projPos, Vector3 axisDir);

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