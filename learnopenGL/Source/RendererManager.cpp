#include "RendererManager.h"

/********************************************************************************
Register renderer comp
********************************************************************************/
void RendererManager::RegisterRenderer(Renderer* regMe)
{
	renderList.push_back(regMe);
}

/********************************************************************************
Pre-update
********************************************************************************/
void RendererManager::PreUpdate()
{
	for (int i = 0; i < renderList.size(); ++i)
	{
		if (renderList[i]->isActive())
			renderList[i]->PreUpdate();
	}
}

/********************************************************************************
update stage 3
********************************************************************************/
void RendererManager::UpdateStage3()
{
	for (int i = 0; i < renderList.size(); ++i)
	{
		if (renderList[i]->isActive())
			renderList[i]->Update();
	}
}

/********************************************************************************
Render renderer
********************************************************************************/
void RendererManager::DrawRenderer()
{
	for (int i = 0; i < renderList.size(); ++i)
	{
		if (renderList[i]->isActive())
			renderList[i]->Draw();
	}
}