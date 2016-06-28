#ifndef INPUT_H
#define INPUT_H
#include "Header.h"

struct TypeChar
{
	char c;
	bool typed;

	TypeChar(char val){ c = val; typed = false; }
	~TypeChar(){}
};

/*******************************************************************************************
Common utilities
*******************************************************************************************/
void SplitString(const string& splitMe, vector<string>& storage, char splitter);

/*******************************************************************************************
Input class: improved version from previous frameworks

Author: Tan Yie Cher
Date: 24/6/2016
*******************************************************************************************/
class Input
{
public:
	/** Put here for controller and model and character to access, got better way? **/
	enum CONTROLS
	{
		W,	//w
		S,	//s
		A,	//a
		D,	//d
		Q,	//q
		SPACE,	//space bar
		C,	//c
		R,	//r
		K,	//k
		L,	//l
		P,	//p
		O,	//o

		/* using mouse/controller... */
		M_LEFT,	//mouse left
		M_RIGHT,	//mouse right
		L_SHIFT,	//shift
		SCROLL_UP,	//mouse scroll
		SCROLL_DOWN,	//mouse scroll
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,

		TOTAL_CONTROLS,
	};

private:

	//Variables--------------------------------------------------//
	unsigned AlphabetMap[O];	//map alphabets to the corr. enum value

	//Mouse variables--------------------------------------------------//
	double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	int m_window_deadzone;
	bool Mouse_DeadZone;	//update cursor with deadzone? if not, no mouse
	int mouseRightButton;
	int mouseLeftButton;
	static double scrollxPos;
	static double scrollyPos;
	Vector3 cursorPos;
	float pitch, yaw;

	//key flags---------------------------------------------------//
	bool KeyPressed[TOTAL_CONTROLS];
	bool KeyHeldDown[TOTAL_CONTROLS];
	bool KeyReleased[TOTAL_CONTROLS];

	//misc flags---------------------------------------------------//
	bool a_key_is_pressed;
	bool a_key_held_down;
	bool a_key_released;

	//Internal functions-------------------------------------------//
	//mouse-----------------------------//
	void UpdateMouse();
	void UpdateCursorPos();

	//Key press------------------------//
	bool checkKeyPress(unsigned short key);

public:

	//Core---------------------------------------------------------------//
	void Init();

	//Mouse settings-----------------------------------------------------//
	void SetMouse(bool activate);

	//Check for key press, CALL AT START OF FRAME AND ONCE ONLY-----------------------------------------------------------//
	void CheckForKeyPresses();

	bool IsKeyPressed(CONTROLS key);
	bool IsKeyHeldDown(CONTROLS key);
	bool IsKeyReleased(CONTROLS key);

	//Getters/setter---------------------------------------------------------------------//
	static void setScrollX(double p);
	static void setScrollY(double p);
	float getYaw();
	float getPitch();
	Vector3 GetCursorPos();
};


#endif