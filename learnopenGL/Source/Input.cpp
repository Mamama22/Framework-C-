#include "Input.h"
#include "View.h"
double Input::scrollxPos = 0.0;
double Input::scrollyPos = 0.0;

/*******************************************************************************************
Split string
*******************************************************************************************/
void SplitString(const string& splitMe, vector<string>& storage, char splitter)
{
	storage.clear();

	for (int start = 0, end = 0, size = 0; end < splitMe.length(); ++size, start = end + 1)
	{
		end = splitMe.find(splitter, start);

		if (end == string::npos && start == splitMe.length())	//if end of string alr, and starting point is the last char
			break;

		if (start != end)	//means next char is also not valid, no numbers in btw
			storage.push_back(splitMe.substr(start, end - start));
	}
}

void scroll(GLFWwindow* window, double x, double y)
{
	Input::setScrollX(x);
	Input::setScrollY(y);
}

bool Input::checkKeyPress(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

void Input::setScrollX(double p)
{
	scrollxPos = p;
}

void Input::setScrollY(double p)
{
	scrollyPos = p;
}

/*******************************************************************************************
Input init
*******************************************************************************************/
void Input::Init()
{
	//Mouse----------------------------------------------------//
	mouse_last_x = mouse_last_y = mouse_current_x = mouse_current_y = mouse_diff_x = mouse_diff_y = 0.0;
	m_window_deadzone = mouseRightButton = mouseLeftButton = 0;
	Mouse_DeadZone = false;
	pitch = yaw = 0.f;

	//Keyboard-------------------------------------------------//
	AlphabetMap[A] = 'A';
	AlphabetMap[B] = 'B';
	AlphabetMap[C] = 'C';
	AlphabetMap[D] = 'D';
	AlphabetMap[E] = 'E';
	AlphabetMap[F] = 'F';
	AlphabetMap[G] = 'G';
	AlphabetMap[H] = 'H';
	AlphabetMap[I] = 'I';
	AlphabetMap[J] = 'J';
	AlphabetMap[K] = 'K';
	AlphabetMap[L] = 'L';
	AlphabetMap[M] = 'M';
	AlphabetMap[N] = 'N';
	AlphabetMap[O] = 'O';
	AlphabetMap[P] = 'P';
	AlphabetMap[Q] = 'Q';
	AlphabetMap[R] = 'R';
	AlphabetMap[S] = 'S';
	AlphabetMap[T] = 'T';
	AlphabetMap[U] = 'U';
	AlphabetMap[V] = 'V';
	AlphabetMap[W] = 'W';
	AlphabetMap[X] = 'X';
	AlphabetMap[Y] = 'Y';
	AlphabetMap[Z] = 'Z';
	AlphabetMap[SPACE] = ' ';


	for (int i = 0; i < TOTAL_CONTROLS; ++i)
		KeyPressed[i] = KeyHeldDown[i] = KeyReleased[i] = false;

	a_key_is_pressed = false;
}

/*******************************************************************************************
check for key presses
*******************************************************************************************/
void Input::UpdateMouse()
{
	glfwGetCursorPos(View::getWindow(), &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;
	//Calculate the yaw and pitch
	yaw = (float)mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	int screen_width = (int)Screen::SCREEN_WIDTH;
	int screen_height = (int)Screen::SCREEN_HEIGHT;
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x >  -m_window_deadzone))
	{
		mouse_current_x = screen_width >> 1;
		glfwSetCursorPos(View::getWindow(), mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > Screen::SCREEN_HEIGHT - m_window_deadzone))
	{
		mouse_current_y = screen_height >> 1;
		glfwSetCursorPos(View::getWindow(), mouse_current_x, mouse_current_y);
	}
	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;
}

/*******************************************************************************************
Update cursor position in camera w/h
*******************************************************************************************/
void Input::UpdateCursorPos()
{
	double x;
	double y;
	glfwGetCursorPos(View::getWindow(), &x, &y);

	cursorPos.Set(x, y);
	int w = Screen::SCREEN_WIDTH;
	int h = Screen::SCREEN_HEIGHT;

	float posX = static_cast<float>(cursorPos.x) / w * Screen::CAMERA_WIDTH;
	float posY = (h - static_cast<float>(cursorPos.y)) / h * Screen::CAMERA_HEIGHT;
	cursorPos = Vector3(posX, posY);
}

/*******************************************************************************************
check for key presses
*******************************************************************************************/
void Input::CheckForKeyPresses()
{
	//Update mouse/cursor position--------------------------------------------//
	if (Mouse_DeadZone)
		UpdateMouse();
	else
		UpdateCursorPos();

	//check for key presses---------------------------------------------------//
	a_key_released = a_key_is_pressed = a_key_held_down = false;

	for (int i = A; i <= SPACE; ++i)
	{
		KeyHeldDown[i] = KeyReleased[i] = false;	//make sure held down and released are ALWAYS SET TO FALSE FOR ALL FRAMES

		bool state_before_check = KeyPressed[i];
		KeyPressed[i] = checkKeyPress(AlphabetMap[i]);

		//if a key is pressed----------------------------------------//
		if (KeyPressed[i])
			a_key_is_pressed = true;

		//if first time pressed, state before latest check is NOT PRESSED-------------------------//
		if (!state_before_check && KeyPressed[i])
		{
			a_key_held_down = KeyHeldDown[i] = true;
		}

		//Previous state is pressed and currently not pressed-------------------------------------//
		else if (state_before_check && !KeyPressed[i])
		{
			a_key_released = KeyReleased[i] = true;
		}
	}
}

/*******************************************************************************************
Enable/disable mouse
*******************************************************************************************/
void Input::SetMouse(bool activate)
{
	Mouse_DeadZone = activate;

	//Set input mode--------------------------------------------------------//
	if (activate)
		glfwSetInputMode(View::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(View::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


/*******************************************************************************************
check the state of the key
*******************************************************************************************/
bool Input::IsKeyPressed(Input::CONTROLS key){ return KeyPressed[key]; }
bool Input::IsKeyHeldDown(CONTROLS key){ return KeyHeldDown[key]; }
bool Input::IsKeyReleased(CONTROLS key){ return KeyReleased[key]; }
float Input::getYaw(){ return yaw; }
float Input::getPitch(){ return pitch; }
Vector3 Input::GetCursorPos(){ return cursorPos; }