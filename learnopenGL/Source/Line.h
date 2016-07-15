#ifndef VECTOR_SE_H
#define VECTOR_SE_H
#include "Header.h"
#include "Mesh.h"

/*************************************************************
Line with start and end

Author: Tan Yie Cher
Date: 13/7/2016
/*************************************************************/
class Line
{
	//utility-----------------------------------------------//
	Vector3 vectorProjection(Vector3& projected, Vector3& dir);

public:

	Vector3 start;
	Vector3 end;
	Vector3 vecMag;	//magnitude
	Vector3 dir;
	float len;

	void Set(float startX, float startY, float endX, float endY);
	void GetProjection(Vector3& dir, Line& projectedVec);
	void Draw(Mesh* line, float thickness);
	void Draw(Mesh* line, float thickness, Vector3 offset);
	void DrawRightLines(Mesh* line, float thickness, Vector3 offset);
};

#endif