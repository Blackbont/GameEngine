#ifndef _WINMAIN_H
#define _WINMAIN_H

//C runtime headers
#ifndef _WINDOWS_
	#include <windows.h>
#endif

int _tmain(int argc, char *argv[]);
int CALLBACK WinMain(HINSTANCE hInstane, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif // _WINMAIN_H