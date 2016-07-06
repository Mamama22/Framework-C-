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
		/************************************** keyboard ****************************************/
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		SPACE,	//space bar

		L_SHIFT,	//shift
		ARROW_UP,
		ARROW_DOWN,
		ARROW_LEFT,
		ARROW_RIGHT,

		/************************************** Mouse ****************************************/
		M_LEFT,	//mouse left
		M_RIGHT,	//mouse right

		TOTAL_CONTROLS,

		/************************************** Special ****************************************/
		SCROLL_UP,	//mouse scroll
		SCROLL_DOWN,	//mouse scroll
	};

private:

	//Variables--------------------------------------------------//
	unsigned AlphabetMap[TOTAL_CONTROLS];	//map alphabets to the corr. enum value

	//Mouse variables--------------------------------------------------//
	double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	int m_window_deadzone;
	bool Mouse_DeadZone;	//update cursor with deadzone? if not, no mouse
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

	//Detect press------------------------//
	bool DetectKeyPress(unsigned short key, CONTROLS keyIndex);

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