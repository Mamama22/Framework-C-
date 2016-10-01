#include "SpriteAnimation.h"
#include "MyMath.h"
#include "GL\glew.h"
#include "Vertex.h"
#include <iostream>
#include "CoreUtilities.h"
using namespace std;

SpriteAnim_Type::SpriteAnim_Type(){}
SpriteAnim_Type::~SpriteAnim_Type(){}

/********************************************************************************
Sprite anim type
********************************************************************************/
void SpriteAnim_Type::Init(TEXTURE_ENUM texture_id, int total_row, int total_col)
{
	this->texture_id = texture_id;
	this->total_row = total_row;
	this->total_col = total_col;
	mesh = MeshBuilder::GenerateQuad_split("", total_row, total_col);
	mesh->SetTexture(texture_id);
}

void SpriteAnim_Type::Exit()
{
	delete mesh;
}

SpriteAnimation::SpriteAnimation() :
  currentRow(0)	
 , currentCol(0)
 ,frameTime(0)
 , timer(0)
 , rendered_prev(false)
{
	//adds itself to manager
	CU::spriteMan.AddActiveSprite(this);
}

void SpriteAnimation::Init(const char* name, Vector3 pos, Vector3 scale, float frameTime, int startCol, 
	int startRow, int endCol, int endRow, SpriteAnim_Type& sprite_type)
{
	/* parent */
	Renderer::Init(name, NULL, pos, scale);

	finishedFrame = false;
	/* set total frame and frame time */

	//start = 0 (min) ||| end = totalRow - 1 (max)
	this->startRow = startRow;
	this->endRow = endRow;
	this->startCol = startCol;
	this->endCol = endCol;

	total_row = (endRow - startRow) + 1;
	total_col = (endCol - startCol) + 1;

	//totalTime / total frame
	//frameTime = (double)(totalTime / (float)(totalRow + totalCol));
	this->frameTime = frameTime;

	/* set current row and column */
	currentRow = startRow;
	currentCol = startCol;

	/* sprite type */
	this->sprite_type = &sprite_type;
}

void SpriteAnimation::ReStart()
{
	currentCol = startCol;
	currentRow = startRow;
}

bool SpriteAnimation::isStart()
{
	return currentCol == startCol &&
	currentRow == startRow;
}

void SpriteAnimation::Update()
{
	rendered_prev = false;

	finishedFrame = false;
	if (timer > 0.0)
		timer -= CU::dt;
	else
	{ 
		timer = frameTime;

		currentCol++;

		//reach the ending row------------//
		if (currentRow == endRow)
		{
			if (currentCol == endCol + 1)	//finish with the end col in end row
			{
				finishedFrame = true;
				currentCol = startCol;
				currentRow = startRow;
			}
		}

		if (currentCol == total_col)
		{
			currentCol = 0;
			currentRow++;
		}
	}
}

void SpriteAnimation::Draw()
{
	CU::view.SetIdentity();
	CU::view.LoadMatrix(transform.finalTRS);
	sprite_type->mesh->Render(GetOffset() * 6, 6, alpha);
	rendered_prev = true;
}

int SpriteAnimation::GetOffset()
{
	return currentCol + (total_col * currentRow);
}

SpriteAnimation::~SpriteAnimation()
{
}
