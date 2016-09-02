#include "Controller.h"


void PrintArray(int gg[])
{
	for (int i = 0; i < 4; ++i)
	{
		cout << gg[i] << endl;
	}
}

int main(void)
{
	/* Memory leak checker */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//call this if program does not exit at same place everytime

	Controller controller;
	controller.Init();
	controller.Run();
	controller.Exit();

	/*int gg[4];
	gg[0] = 1;
	gg[1] = 2;
	gg[2] = 3;
	gg[3] = 4;
	PrintArray(gg);*/
}