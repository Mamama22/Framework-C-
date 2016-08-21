#include "SpriteAnimation.h"
#include "MyMath.h"
#include "GL\glew.h"
#include "Vertex.h"
#include <iostream>
#include "CoreUtilities.h"
using namespace std;

SpriteAnimation::SpriteAnimation(int row, int col)
 : total_row(row)
 , total_col(col)
 , currentRow(0)	
 , currentCol(0)
 ,frameTime(0)
 , timer(0)
 , rendered_prev(false)
{
	//adds itself to manager
	CU::spriteMan.AddActiveSprite(this);
}

void SpriteAnimation::init(float frameTime, int startCol, int startRow, int endCol, int endRow)
{
	finishedFrame = false;
	/* set total frame and frame time */

	//start = 0 (min) ||| end = totalRow - 1 (max)
	this->startRow = startRow;
	this->endRow = endRow;
	this->startCol = startCol;
	this->endCol = endCol;

	int totalRow = (endRow - startRow) + 1;
	int totalCol = (endCol - startCol) + 1;

	//totalTime / total frame
	//frameTime = (double)(totalTime / (float)(totalRow + totalCol));
	this->frameTime = frameTime;

	/* set current row and column */
	currentRow = startRow;
	currentCol = startCol;
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

void SpriteAnimation::Render()
{
	Mesh::Render(GetOffset() * 6, 6);
	rendered_prev = true;
}

int SpriteAnimation::GetOffset()
{
	return currentCol + (total_col * currentRow);
}

SpriteAnimation::~SpriteAnimation()
{
}
