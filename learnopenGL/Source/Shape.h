#ifndef SHAPE_H
#define SHAPE_H

#include "Renderer.h"
#include "Line.h"

/*************************************************************
Point class

Author: Tan Yie Cher
Date: 16/7/2016
/*************************************************************/
class Point
{
public:

	Vector3 pos;
	Vector3 offset;	//offset from shape

	Point();
	Point(const Point& copy);
	~Point();

	void Set(Vector3 pos, Vector3 shapePos);
};

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

	unsigned start, end;	//index to start and end point
	float len;
	float angle;
	Vector3 dir;
	Vector3 normal;

	/******************** core functions **********************/
	void Set(int startPoint_index, int endPoint_index, vector<Point>& pointList, Vector3 shapePos);
	void Rotate(float angle);
	void Draw(vector<Point>& pointList);
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
public:

	/******************************************** var ***************************************************/
	vector<Point> pointList;
	vector<Face> faceList;	//list of points

	/******************** constructor/destructor **********************/
	Shape();
	Shape(const Shape& copyMe);
	~Shape();

	/******************** Transformation: added on to parent's **********************/
	void Translate(Vector3 vel);	
	void Rotate(float angle);

	/******************** core functions **********************/
	void Init(const char* name);
	void AddPoint(Vector3 pos);
	void CalculateFaces();	//CALL AFTER ALL POINTS ADDED

	void GetProjections(Vector3& dir, Vector3 list[]);

	void RecalculatePoints();
	void Draw();	//draws the outline of the shape


	/******************** Get set functions **********************/
	int Get_TotalPoints();
};

#endif