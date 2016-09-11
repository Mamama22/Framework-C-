#ifndef TILEMAP_H
#define TILEMAP_H
#include "Mesh.h"


/*************************************************************
Tilemap class

Author: Tan Yie Cher
Date: 11/9/2016

Note:
-IMPORTANT: render mesh of this type without binding to any other external VAO, ie. text VAO!!!
-DO not change tilemap id once texture is set
/*************************************************************/

class TileMap
{
	int tilesX;
	int tilesY;
	GLuint id;

public:

	void Init(int tilesX, int tilesY, GLuint id)
	{
		this->tilesX = tilesX; 
		this->tilesY = tilesY;
		this->id = id;
	}

	int total_tilesX(){ return tilesX; }
	int total_tilesY(){ return tilesY; }
	int total_tiles(){ return tilesX * tilesY; }
	GLuint getID(){ return id; }
};

#endif