#ifndef SPRITE_ANIM_MAN_H
#define SPRITE_ANIM_MAN_H

#include "Header.h"
#include "SpriteAnimation.h"

/*************************************************************
sprite animation manager

Author: Tan Yie Cher
Date: 22/8/2016

Updates any sprite animation meshes
/*************************************************************/
class SpriteEAnimMan
{
	vector<SpriteAnimation*> spriteList;

public:

	void AddActiveSprite(SpriteAnimation* addMe){ spriteList.push_back(addMe); }

	void Update()
	{
		for (int i = 0; i < spriteList.size(); ++i)
		{
			if (spriteList[i]->rendered_prev)	//only update if currently being rendered
				spriteList[i]->Update();
		}
	}
};

#endif