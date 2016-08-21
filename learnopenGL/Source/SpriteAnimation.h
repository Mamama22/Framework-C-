#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H
#include "Mesh.h"
#include "Vector2.h"

/*** mesh class for animation ***/
class SpriteAnimation : public Mesh
{

public:

	/* constructor/destructor */
	 SpriteAnimation(int row, int col);
	 ~SpriteAnimation();

	/* init/update/render */
	 //oppDir: if true, going from bottom to top and right to left
	void init(float frameTime, int startCol, int startRow, int endCol, int endRow);
	void Update();
	void Render();
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

	double frameTime;		//time per frame
	double timer;

	int currentRow;		//keep track of current row
	int currentCol;	//keep track of current col

	bool finishedFrame;


	bool rendered_prev;	//is it rendered prev? update if so
};

#endif