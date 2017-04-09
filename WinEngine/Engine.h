#ifndef _ENGINE_H
#define _ENGINE_H

#include "Game.h"
//C runtime headers
#ifndef _WINDOWS_
	#include <windows.h>
#endif

#ifndef _MAP_
	#include <map>
#endif

#ifndef _CONTEXT_H
	#include "context.h"
#endif

//Enum
enum EngineState
{
	Invalid,
	Constructing,
	Initializing,
	Running,
	ShuttingDown,
	Destroing
};

class Engine
{
public:
	Engine(void);
	~Engine(void);

	//void* operator new(size_t size);
	//void operator delete(void* pointer);
	int RunLoop();
	static EngineState GetEngineState() { return engineState; }
private:
	int Initialize();
	int Draw(Context &context);
	int Update(Context &context);
	int ShutDown();

	//Добавление системы в движок
	int AddSystem(USystem *system);
	//Извлечение системы из движка
	template<typename T>
	T* GetSystem(SystemType sysType);

	//Create game instance
	Game *CreateGame();

	std::map<SystemType, USystem*> mapSystems;
	std::map<SystemType, USystem*>::iterator it;
	static EngineState engineState;

};

#endif // _ENGINE_H