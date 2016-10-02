#ifndef CONTROLLER_H
#define CONTROLLER_H

//Scenes-------------------------//
#include "Scene_Boxhead.h"
#include "Scene_Test.h"
#include "Scene_MapEditor.h"

/*************************************************************
New and improved controller

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class Controller
{
	//Scenes--------------------------------//
	Scene* current_scene;
	Scene_Boxhead scene_boxhead;
	Scene_Test scene_test;
	Scene_MapEditor scene_mapeditor;

	/********************** FPS **********************/
	const static unsigned char Controller::FPS; // FPS of this game (ONLY RELEASE MODE CAN RUN SMOOTHLY AT 170FPS MAX)
	const static unsigned int Controller::frameTime; // time for each frame

	/********************** FPS calculations **********************/
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_thread1;
	double m_dAccumulatedTime_thread2;
	float fps;

public:

	/******************** core functions **********************/
	void Init();
	void Run();
	void Exit();
};

#endif