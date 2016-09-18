#include "CoreUtilities.h"

View CU::view;
Input CU::input;
SharedResources CU::shared;
EntityManager CU::entityMan;
RendererManager CU::rendererMan;
SpriteEAnimMan CU::spriteMan;
double CU::dt = 0.0;
float CU::fps = 0.f;

/********************************************************************************
Init core utilities
********************************************************************************/
void CU::Init()
{
	input.Init();
	view.Init();
	shared.Init();
	entityMan.Init();
}

/********************************************************************************
Init core utilities after scene's init
********************************************************************************/
void CU::Post_Init()
{
	//set TRS for all----------------------------//
	entityMan.UpdateStage2();
}

/********************************************************************************
Pre update
********************************************************************************/
void CU::PreUpdate()
{
	//Sprite---------------------------------------//
	spriteMan.Update();

	entityMan.PreUpdate();
	rendererMan.PreUpdate();

	//Transformation---------------------------------------//
	Transformation::TRS_count = 0;
	Transformation::TRS_cal_count = 0;
}

/********************************************************************************
update
********************************************************************************/
void CU::Update_Stage1()
{
	entityMan.UpdateStage1();
}

/********************************************************************************
update
********************************************************************************/
void CU::Update_Stage2()
{
	entityMan.UpdateStage2();
}

/********************************************************************************
update
********************************************************************************/
void CU::Update_Stage3()
{
	entityMan.UpdateStage3();
	rendererMan.UpdateStage3();
}

/********************************************************************************
update
********************************************************************************/
void CU::Update_Stage4()
{
	entityMan.UpdateStage4();
}

/********************************************************************************
Render
********************************************************************************/
void CU::DrawOnScreen()
{
	//renderer are special component that needs to be drawn
	rendererMan.DrawRenderer();
}

/********************************************************************************
update
********************************************************************************/
void CU::Exit()
{
	view.Exit();
	shared.Exit();
	entityMan.Exit();
}