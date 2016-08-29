#ifndef COMP_H
#define COMP_H

#include "MeshBuilder.h"
#include "Component.h"

/*==========================================================================================================================
Renderer comp, must have to render shit

Type: abstract class
Usage: inherit to define your own special renderer

Author: Tan Yie Cher
Date: 4/7/2016
/===========================================================================================================================*/

class Renderer : public Component
{
protected:

	/********************** var **********************/
	Mesh* mesh;

public:

	/******************** constructor/destructor **********************/
	Renderer();
	Renderer(const Renderer& copyMe);
	~Renderer();

	/******************** comp functions **********************/
	void Init(const char* name, Mesh* mesh, Vector3 pos, Vector3 scale);

	/******************** abstract functions **********************/
	virtual void Draw() = 0;	//All renderer need it, called after all stages finished
	void Exit();
};

/*==========================================================================================================================
Render on screen

Author: Tan Yie Cher
Date: 4/7/2016
/===========================================================================================================================*/
class Render_OnScreen : public Renderer
{
public:

	/******************** abstract functions **********************/
	void Draw();	//render
};

/*==========================================================================================================================
Render in world

Author: Tan Yie Cher
Date: 4/7/2016
/===========================================================================================================================*/
class Render_InWorld : public Renderer
{
public:

	/******************** abstract functions **********************/
	void Draw();	//render
};

/*==========================================================================================================================
Render on screen repetitive

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
	void ModifyTile(int x, int y, int tileType, int tilemap_sizeX, int tilemap_sizeY);
	void RecalculateMesh();	//call after modifying tiles

	/******************** abstract functions **********************/
	void Draw();	//render
};



/*==========================================================================================================================
Render on screen repetitive

How to use:
1) Call init and pass in relevant info, make sure mesh is a tilemap

Author: Tan Yie Cher
Date: 26/8/2016
/===========================================================================================================================*/
class Render_Repetitive : public Renderer
{
	Vector3 offset;
	int count;
	
public:

	/******************** comp functions **********************/
	void Init(const char* name, Mesh* mesh, Vector3 pos, Vector3 scale, Vector3 offset, int count);

	/******************** abstract functions **********************/
	void Draw();	//render
};

#endif