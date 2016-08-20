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
	static Vector3 shareVec;

public:

	Face();
	Face(const Face& copy);
	~Face();

	unsigned start, end;	//index to start and end point handle
	float len;	//length from start to end
	float angle;	//angle of dir.
	Vector3 dir;	//dir
	Vector3 normal;	//normal, points outwards

	/**************************************** core functions ******************************************/
	void Set(int startPoint_index, int endPoint_index, vector<Point>& pointList);
	void Rotate(float angle);
	void Draw(vector<Point>& pointList);

	/**************************************** Line intersection ******************************************/
	static bool intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t);
	bool LineIntersection(const Vector3& lineOrigin, float lineAngle, vector<Point>& pList, Vector3& intersectedPos);
};

/*************************************************************
Shape class with SAT collision

Instructions:
-NO CONCAVE SHAPES, ONLY CONVEX
-points will be linked in order of list

Issues:
-Shape* parent, move to private and make int (handle)

Author: Tan Yie Cher
Date: 16/7/2016
/*************************************************************/
class Shape : public Renderer
{
	/************************************* Projection/Collision functions ***************************************/
	void GetProjections(Vector3& dir, float list[]);
	bool SAT_CollisionCheck(Shape& checkMe, Vector3& normal, float& bounce, bool thisShape, float& offsetDistSq);
	static void ProjectOntoNormal(Shape& projected, const Vector3& normal, float store[]);
	static bool IntersectionTest(float proj_1[], float proj_2[], float& intersectedLen);
	void TranslatePosWithAngle(Vector3& pos, Vector3 dir, float speed);


	/************************************* optional abstract functions to overload  ***************************************/
	void Added_ToEntity(int handle);

public:

	vector<Shape*> childrenShapes;
	Shape* parent_shape;		//move to private, change to int

	/******************************************** Collision related ***************************************************/
	vector<Point> pointList;
	vector<Face> faceList;	//list of points
	Vector3 prevPos;
	Vector3 vel;

	//flags------------------//
	bool collided;
	bool collide_withParent;

	/******************** constructor/destructor **********************/
	Shape();
	Shape(const Shape& copyMe);
	~Shape();

	/******************** Transformation: added on to parent's **********************/
	//transformation
	void Translate(Vector3 vel);	
	void Rotate(float angle);

	//call when recalculating TRS
	void RecalculatePoints();

	/******************** Init functions **********************/
	void Init(const char* name, Vector3 pos);
	void AddPoint(Vector3 pos);
	void CalculateFaces();	//CALL AFTER ALL POINTS ADDED

	/******************** collision functions **********************/
	void CollisionCheck_2(Shape& obstacle);

	/******************** CALLED BY PARENT ENTITY ONLY **********************/
	//transformation---------------------------------------------//
	void ByParent_Rotate(float angle, Vector3 axis);

	void CalculateTRS_WithParent(const Mtx44& parentRotMat, bool GrandParentTransform);

	/************************************* Projection functions ***************************************/
	void ProjectShapeOntoThis(Shape& projectMe, float** list);	//project passed in shape onto this shape
	
	/******************** Core functions **********************/
	void PreUpdate();
	void Update();
	void Draw();

	/******************** Get set functions **********************/
	int Get_TotalPoints();

	/************************************* Line intersection ***************************************/
	bool CheckLineIntersection(const Vector3& lineOrigin, float lineAngle, Vector3& intersectedPos);
};

#endif