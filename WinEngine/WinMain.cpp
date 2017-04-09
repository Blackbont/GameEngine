#include "WinMain.h"

//Additional includes
#include "Engine.h"

#ifndef _STRING_H
	#include "string.h"
#endif
#ifndef _IOSTREAM_H
	#include "iostream.h"
#endif
#ifndef _DELETEMACROS_H
	#include "DeleteMacros.h"
#endif

#if defined (DEBUG) | defined (_DEBUG) // Дебаг версия с консольным окном для отладки, работает только в DEBUG Режиме 
int _tmain(int argc, char *argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	//Logger::Log("Start the programm");

	WinMain((HINSTANCE)GetModuleHandle(NULL), 0, 0, SW_SHOW);
}
#endif

int WINAPI WinMain(HINSTANCE hInstane, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstane);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

#if defined(DEBUG) | defined(_DEBUG) //Отслеживание утечек памяти
	/*HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(0);*/
#endif

	//Создание движка
	Engine *engine = new Engine();

	//Запуск игрового цикла
	int result = engine->RunLoop();

	//Удаление движка
	SafeDelete(engine);
}