#include "Controller.h"

int main(void)
{
	/* Memory leak checker */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//call this if program does not exit at same place everytime

	Controller controller;
	controller.Init();
	controller.Run();
	controller.Exit();
}