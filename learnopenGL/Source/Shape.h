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
	/************************************* Projection functions ***************************************/
	void GetProjections(Vector3& dir, float list[]); //get list of projection of points of this shape on a axis

	/************************************* Collision check ***************************************/
	//Project shape 'checkMe' onto this and check
	bool SAT_CollisionCheck(Shape& checkMe, Vector3& normal, float& bounce, bool thisShape, float& offsetDistSq);
	static void ProjectOntoNormal(Shape& projected, const Vector3& normal, float store[]);
	static bool IntersectionTest_2(float proj_1[], float proj_2[], float& intersectedLen);
	void TranslatePosWithAngle(Vector3& pos, Vector3 dir, float speed);

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
	//transformation
	void Translate(Vector3 vel);	
	void Rotate(float angle);

	//call when recalculating TRS
	void RecalculatePoints(bool debug);

	/******************** Init functions **********************/
	void Init(const char* name, Vector3 pos);
	void AddPoint(Vector3 pos);
	void CalculateFaces();	//CALL AFTER ALL POINTS ADDED

	/******************** collision functions **********************/
	void CollisionCheck_2(Shape& obstacle);

	/************************************* Projection functions ***************************************/
	void ProjectShapeOntoThis(Shape& projectMe, float** list);	//project passed in shape onto this shape

	void CalculateTRS_WithParent(const Mtx44& parentRotMat);
	
	/******************** Core functions **********************/
	void Update();
	void Draw();

	/******************** Get set functions **********************/
	int Get_TotalPoints();
};

#endif