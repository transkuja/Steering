#include "App.h"

#include <ctime>

#define _CRT_SECURE_NO_WARNINGS

#define FRAMERATE 50

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

int main(int argc, char** argv)
{
	App myApp;

	myApp.Init();

	double timer = clock();
	while(1)
	{
		if(clock() - timer > FRAMERATE)
		{
			myApp.Update(FRAMERATE * 0.001f);
			timer = clock();
		}
	}
	myApp.DeInit();

	return 0;
}
