#ifndef COMP_H
#define COMP_H

#include "Mesh.h"
#include "Component.h"

/*************************************************************
Renderer comp, must have to render shit

Type: abstract class
Usage: inherit to define your own special renderer

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Renderer : public Component
{
protected:

	/********************** var **********************/
	Mesh* mesh;
	Vector3 dir;

public:

	/******************** constructor/destructor **********************/
	Renderer();
	Renderer(Renderer& copyMe);
	~Renderer();

	/******************** comp functions **********************/
	void Init(const char* name, Mesh* mesh, Vector3 pos, Vector3 scale);

	/******************** abstract functions **********************/

	void Exit();
};

/*************************************************************
Render on screen

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Render_OnScreen : public Renderer
{
public:

	/******************** abstract functions **********************/
	void UpdatingComp();	//render
};

/*************************************************************
Render in world

Author: Tan Yie Cher
Date: 4/7/2016
/*************************************************************/
class Render_InWorld : public Renderer
{
public:

	/******************** abstract functions **********************/
	void UpdatingComp();	//render
};

#endif