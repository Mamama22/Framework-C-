#include "Controller.h"
const unsigned char Controller::FPS = 1000; // FPS of this game
const unsigned int Controller::frameTime = 1000 / FPS; // time for each frame

/********************************************************************************
Init stuff
********************************************************************************/
void Controller::Init()
{
	//Timers: set both timers to 0-----------------------------//
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_thread1 = 0.0;
	m_dAccumulatedTime_thread2 = 0.0;

	//rand
	srand(time(NULL));

	//Core utils init------------------------------//
	CU::Init();
	CU::input.SetMouse(false);	//no mouse used

	//Init scene-----------------------------------//
	//scene_boxhead
	//scene_test
	current_scene = &scene_boxhead;
	current_scene->Init();

	CU::Post_Init();
}

/********************************************************************************
Run stuff
********************************************************************************/
void Controller::Run()
{
	//Main Loop
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	double aTime = 0;
	bool b = false;

	//quit owhen previous frame initiates
	while (!glfwWindowShouldClose(View::getWindow()) && !CU::input.IsKeyPressed(Input::Q))
	{
		//=============================================================================================================================================//
		//get the elapsed time--------------------------------------//
		CU::dt = m_dElapsedTime = m_timer.getElapsedTime();
		aTime += m_dElapsedTime;
		m_dAccumulatedTime_thread1 += m_dElapsedTime;
		m_dAccumulatedTime_thread2 += m_dElapsedTime;
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		//fps------------------------------------------------//
		CU::fps = fps = (float)(1.f / m_dElapsedTime);
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		/* Update */
		if (m_dAccumulatedTime_thread1 > 0.01)	//update: update fps is _dAccumulatedTime_thread1 > fps
		{
			m_dAccumulatedTime_thread1 = 0.0;
	
			//call before all-----------------------------------------//
			CU::input.CheckForKeyPresses();

			//CU pre update-----------------------------------//
			CU::PreUpdate();

			//Scene update-------------------------------------------//
			current_scene->Run_Stage1();

			CU::Update_Stage1();

			CU::Update_Stage2();

			current_scene->Run_Stage3();

			CU::Update_Stage3();

			CU::Update_Stage4();
			
			//check if any input pressed------------------------------//
			if (fps <= 40.f)
				cout << fps << endl;
		}
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		/* Render */
		if (m_dAccumulatedTime_thread2 > 0.005)	//render: render fps is _dAccumulatedTime_thread1 > fps
		{
			CU::view.ClearScreen();

			//World Render-----------------------------------------------------------------------//
			CU::view.Start_WorldRender(current_scene->camera);

			current_scene->DrawInWorld();

			//Screen Render-----------------------------------------------------------------------//
			CU::view.Start_ScreenRender(-Screen::CAMERA_WIDTH*0.5f, -Screen::CAMERA_HEIGHT*0.5f);

			CU::DrawOnScreen();	//framework draw on screen
			current_scene->DrawOnScreen();

			//GUI Render-------------------------------------------------------------------------//
			current_scene->DrawGUI();

			//Post Render-----------------------------------------------------------------------//
			CU::view.PostRender();


			m_dAccumulatedTime_thread2 = 0.0;
		}
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		//Swap buffers
		glfwSwapBuffers(View::getWindow());
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		//=============================================================================================================================================//

		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   
	}
}

/********************************************************************************
Exit stuff
********************************************************************************/
void Controller::Exit()
{
	//Scene exit------------------------------------//
	current_scene->Exit();

	//Core utils exit------------------------------//
	CU::Exit();
}