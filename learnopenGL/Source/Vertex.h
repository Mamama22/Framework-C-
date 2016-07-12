#ifndef VERTEX_H
#define VERTEX_H

#include "Vector3.h"
#include "Header.h"

struct Position
{
	float x, y, z;
	Position(float x = 0, float y = 0, float z = 0) { Set(x, y, z); }
	void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
};

struct Color
{
	float r, g, b;
	Color(float r = 255.f, float g = 255.f, float b = 255.f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b) { this->r = r / 255.f; this->g = g / 255.f; this->b = b / 255.f; }

	Color& operator= (Color& copyMe){
		r = copyMe.r;g = copyMe.g;b = copyMe.b;
		return *this;
	}
};

struct Color4
{
	float r, g, b, a;
	Color4(float r = 255, float g = 255, float b = 255, float a = 1) { Set(r, g, b, a); }
	void Set(float r, float g, float b, float a) { this->r = r / 255.f; this->g = g / 255.f; this->b = b; this->a = a; }
};


struct TexCoord
{
	float u, v;
	TexCoord(float u = 0, float v = 0) { Set(u, v); }
	void Set(float u, float v) { this->u = u; this->v = v; }
};

/*************************************************************
Vertex

IMPORTANT: Do not change layout of attributes!!

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
struct Vertex
{
	Position pos;
	Color color;
	Vector3 normal;

	Vertex(){}
};

#endif