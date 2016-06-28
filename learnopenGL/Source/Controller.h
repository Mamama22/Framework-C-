#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "CoreUtilities.h"
#include "FPS_Camera.h"
#include "MeshBuilder.h"

/*************************************************************
New and improved controller

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
class Controller
{
	//camera--------------------------------//
	FPS_Cam camera;

	Mesh* axes;
	Mesh* quad;
	Mesh* sphere;

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