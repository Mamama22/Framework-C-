#include "Controller.h"

int main(void)
{
	/* Memory leak checker */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//call this if program does not exit at same place everytime

	Controller controller;
	controller.Init();
	controller.Run();
	controller.Exit();

	/*int** asdsd;
	asdsd = new int*[20];

	for (int i = 0; i < 20; ++i)
	{
		asdsd[i] = new int[20];

		for (int y = 0; y < 20; ++y)
		{
			asdsd[i][y] = 22;
		}
	}
	cout << "ASDAS: " << asdsd[12][13] << endl;*/
}