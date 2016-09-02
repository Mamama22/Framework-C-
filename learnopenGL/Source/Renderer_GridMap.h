#ifndef RENDERER_GRIDMAP_H
#define RENDERER_GRIDMAP_H
#include "Renderer.h"

/*==========================================================================================================================
Render grid map

How to use:
1) Call init and pass in relevant info, make sure mesh is a tilemap

Author: Tan Yie Cher
Date: 26/8/2016
/===========================================================================================================================*/
class Render_GridMap : public Renderer
{
	//mesh stuff-----------------------------------------------------------------//
	vector<Vertex> vertex_buffer_data;
	vector<GLuint> index_buffer_data;

public:

	int totalX_tiles;
	int totalY_tiles;
	int** gridmap;
	float tileScale;
	TEXTURE_ENUM tileMap_tex;

	Render_GridMap();
	Render_GridMap(const Render_GridMap& copyMe);
	~Render_GridMap();

	/******************** comp functions **********************/
	void Init(const char* name, TEXTURE_ENUM tileMap_tex, Vector3 pos, float tileScale, int totalX_tiles, int totalY_tiles);

	/******************** Core functions **********************/
	void SetTile(int x, int y, int tileType, int tilemap_sizeX, int tilemap_sizeY);
	void RecalculateMesh();	//call after modifying tiles

	/******************** Utilities functions **********************/
	void SetAllTiles(int tileType, int tilemap_sizeX, int tilemap_sizeY);
	void SetTileEmpty(int x, int y);
	void SetAllTilesEmpty();

	/******************** abstract functions **********************/
	void Draw();	//render
};

#endif