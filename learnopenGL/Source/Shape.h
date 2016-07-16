#ifndef SHAPE_H
#define SHAPE_H

#include "Renderer.h"
#include "Line.h"

/*************************************************************
face: contains necessary info for a face in a shape

Author: Tan Yie Cher
Date: 16/7/2016
/*************************************************************/
class Face
{
	/******************** static var **********************/
	static Vector3 shareVec;

public:

	Face();
	Face(const Face& copy);
	~Face();

	Vector3 start;
	float len;
	float angle;
	Vector3 dir;
	Vector3 normal;
	Vector3 offset;	//offset from shape pos

	/******************** core functions **********************/
	void Set(Vector3 start, Vector3 end, Vector3 shapePos);
	void Rotate(float angle);
	void Draw();
};

/*************************************************************
Shape class with SAT collision

Instructions:
-NO CONCAVE SHAPES, ONLY CONVEX
-points will be linked in order of list

Author: Tan Yie Cher
Date: 16/7/2016
/*************************************************************/
class Shape : public Renderer
{

	/******************************************** var ***************************************************/
	vector<Face> faceList;	//list of points

public:

	/******************** constructor/destructor **********************/
	Shape();
	Shape(const Shape& copyMe);
	~Shape();

	/******************** Transformation: added on to parent's **********************/
	void Translate(Vector3 vel);	
	void Rotate(float angle);

	/******************** core functions **********************/
	void Init(const char* name);
	void AddFace(Vector3 start, Vector3 end);

	void GetProjection(Vector3& dir, Line& projectedVec);

	void Draw();	//draws the outline of the shape
};

#endif