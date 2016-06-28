#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#ifndef HEADER_H
#define HEADER_H
/*************************************************************
All includes here

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
#include <iostream>
#include "Mtx44.h"
#include "Vector3.h"
#include "MatrixStack.h"
#include <vector>

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//freetype
#include <ft2build.h>
#include FT_FREETYPE_H

#include "timer.h"
#include "MyMath.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "GL\glew.h"
#include "shader.hpp"
#include <map>
#include <sstream>

using namespace std;

//screen dimensions----------------------------------//
//small screen size: 800, 600
//big screen size: 1000, 750
//Extra big: 1150, 862.5
class Screen
{
	static float zoom_percent;
public:
	static float SCREEN_WIDTH;
	static float SCREEN_HEIGHT;

	// Viewing dimensions (2D/3D) ------------------------/
	const static float CAM_X_TO_Y_RATIO;
	static unsigned short CAMERA_WIDTH;	//camera view size X 
	static unsigned short CAMERA_HEIGHT;	//camera view size Y
};

#endif