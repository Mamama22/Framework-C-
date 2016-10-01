#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H
#include "Renderer.h"
#include "Vector2.h"
/*************************************************************
Sprite animation type. Contains all the basic info of a sprite type

Author: Tan Yie Cher
Date: 26/9/16
/*************************************************************/
class SpriteAnim_Type
{
public:

	TEXTURE_ENUM texture_id;
	int total_row;
	int total_col;
	Mesh* mesh;	//shared mesh

	SpriteAnim_Type();
	~SpriteAnim_Type();

	void Init(TEXTURE_ENUM texture_id, int total_row, int total_col);
	void Exit();
};

/*************************************************************
Sprite animation
modifed to not be mesh.
Has 2 managers.

Author: Tan Yie Cher
Date: early 2015
/*************************************************************/
/*** mesh class for animation ***/
class SpriteAnimation : public Renderer
{

public:

	/* constructor/destructor */
	SpriteAnimation();
	~SpriteAnimation();

	/* init/update/render */
	//oppDir: if true, going from bottom to top and right to left
	void Init(const char* name, Vector3 pos, Vector3 scale, float frameTime, int startCol, int startRow, int endCol, int endRow, SpriteAnim_Type& sprite_type);
	void Update();

	//Virtual-------------------------------------------------------------------//
	void Draw();

	int GetOffset();	//for rendering

	void ReStart();
	bool isStart();

	/* variables */
	int total_row;
	int total_col;
	int startRow;
	int endRow;
	int startCol;
	int endCol;
	SpriteAnim_Type* sprite_type;

	double frameTime;		//time per frame
	double timer;

	int currentRow;		//keep track of current row
	int currentCol;	//keep track of current col

	bool finishedFrame;


	bool rendered_prev;	//is it rendered prev? update if so
};

#endif