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

	unsigned start, end;	//index to start and end point handle
	float len;
	float angle;
	Vector3 dir;
	Vector3 normal;

	/******************** core functions **********************/
	void Set(int startPoint_index, int endPoint_index, vector<Point>& pointList, Vector3 shapePos, bool debug = false);
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
	//this proj. onto obstacle----------------------------//
	static float** shapeProjPoints;	//to store shape projected points
	static float** shapeProjPoints_2ndCheck;	//to store shape projected points for checking another shape

	//obstacle proj. to this----------------------------//
	static float** shapeProjPoints_2;	//to store shape projected points
	static float** shapeProjPoints_2_2ndCheck; //to store shape projected points for checking another shape

	//store min max-------------------------//
	static float** minMax_this;
	static float** minMax_this_2ndCheck;

	static float** minMax_projected;
	static float** minMax_projected_2ndCheck;

public:

	/******************************************** var ***************************************************/
	vector<Point> pointList;
	vector<Face> faceList;	//list of points
	Vector3 prevPos;
	Vector3 vel;

	/******************** constructor/destructor **********************/
	Shape();
	Shape(const Shape& copyMe);
	~Shape();

	/******************** Transformation: added on to parent's **********************/
	void Translate(Vector3 vel);	
	void Rotate(float angle);
	void RecalculatePoints(bool debug);

	/******************** Init functions **********************/
	static void InitStatic();
	void Init(const char* name, Vector3 pos);
	void AddPoint(Vector3 pos);
	void CalculateFaces();	//CALL AFTER ALL POINTS ADDED

	/******************** collision functions **********************/
	void CollisionCheck(Shape& obstacle);
	void GetMinMax(Shape& projected, float** projectedPoints, float** storeHere);	//returns smallest intersection
	bool IntersectionTest(float** minMax_This, float** minMaxProjected, Vector3& axisDir, float& bounceVal);

	/******************** Projection functions **********************/
	void ProjectShapeOntoThis(Shape& projectMe, float** list);	//project passed in shape onto this shape
	void GetProjections(Vector3& dir, float list[]); //get list of projection on a axis
	
	/******************** Draw functions **********************/
	void Update();
	void Draw();

	/******************** Get set functions **********************/
	int Get_TotalPoints();
};

#endif