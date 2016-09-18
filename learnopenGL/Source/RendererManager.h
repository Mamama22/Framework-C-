#ifndef RENDERER_MANAGER_H
#define RENDERER_MANAGER_H

#include "Renderer.h"

/*************************************************************
Entity manager. Only stores entities. DOES NOT UPDATE THEM

Type: manager class
Usage: Entity adds itself to this and gets a handle

Author: Tan Yie Cher
Date: 28/8/2016
/*************************************************************/
class RendererManager
{
protected:

	/******************** var **********************/
	vector<Renderer*> renderList;

public:

	/******************** Update functions **********************/
	void PreUpdate();
	void UpdateStage3();

	/******************** comp functions **********************/
	void RegisterRenderer(Renderer* regMe);

	/******************** Draw Renderer **********************/
	void DrawRenderer();
};


#endif