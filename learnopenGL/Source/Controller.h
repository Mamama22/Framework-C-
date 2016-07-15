#ifndef CONTROLLER_H
#define CONTROLLER_H

//Scenes-------------------------//
#include "Scene_Test.h"
#include "Scene_ECS.h"
#include "Scene_Collision.h"
#include "Scene_SAT_Test.h"

/*************************************************************
New and improved controller

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class Controller
{
	//Scenes--------------------------------//
	Scene* current_scene;
	Scene_Test sceneTest;
	Scene_ECS sceneECS;
	Scene_Collision sceneCollision;
	Scene_SAT_Test scene_SAT_Test;

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