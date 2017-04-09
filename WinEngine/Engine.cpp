#include "Engine.h"

//Additional includes
#include "USystem.h"
#include "Game.h"
#include "Window.h"

#ifndef _STRING_H
	#include "string.h"
#endif

#ifndef _DELETEMACROS_H
	#include "DeleteMacros.h"
#endif
//C Runtime headers


EngineState Engine::engineState = EngineState::Invalid;
//Открытые методы
#pragma region Public methods
Engine::Engine(void)
{
	engineState = EngineState::Constructing;
}


Engine::~Engine(void)
{
	engineState = EngineState::Destroing;
}

int Engine::RunLoop()
{
	srand(GetTickCount());

	Context context;

	if(!this->Initialize())
		return 0;

	MSG msg = {};

	engineState = EngineState::Running;

	while(msg.message != WM_QUIT && engineState == EngineState::Running)
	{
		//CheckResize();

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		this->Update(context);
		this->Draw(context);
	}	

	//Logger::Log("Ending the program");
	//Logger::SaveToFile();

	if(!this->ShutDown())
		return 0;

	return msg.wParam;
}

#pragma endregion

//Закрытые методы
#pragma region Private methods
int Engine::Initialize()
{
	engineState = EngineState::Initializing;

	Game* game = CreateGame();

	if(!game) return false;

	//Добавлять системы сюда
	if(!AddSystem(new Window(WindowData(640, 480))))
		return false;

	//Инициировать системы здесь
	if(!mapSystems[SystemType::System_Window]->Initialize())
		return false;

	return true;
}

int Engine::Draw(Context &context)
{

	return true;
}

int Engine::Update(Context &context)
{
	for(it = mapSystems.begin(); it != mapSystems.end(); ++it)
	{
		if(!it->second) 
			continue;
		
		USystem *temp = it->second;
			temp->Update(context);
	}
	return true;
}

int Engine::ShutDown()
{
	engineState = EngineState::ShuttingDown;

	for(it = mapSystems.begin(); it != mapSystems.end(); ++it)
	{
		/*if(!(*it).second->ShutDown())
		{
			//Log::Logger("Failed to shutdown system!" + GetSystemType());
			continue;
		}
		SafeDelete(it->second);*/
	}
	return true;
}

template<typename T>
T* Engine::GetSystem(SystemType sysType)
{
	T *psys = static_cast<T*>(mapSystems[sysType]);
	if(!psys)
	{
		//Logger::Log("System is not valid!");
		return nullptr;
	}
	return psys;
}

int Engine::AddSystem(USystem *system)
{
	auto element = mapSystems.insert(std::make_pair(system->GetType(), system));

	if(element.second)
	{
		return true;
	}

	return false;
}

Game *Engine::CreateGame()
{
	if(!AddSystem(new Game(GameData())))
		return nullptr;

	Game *game = GetSystem<Game>(SystemType::System_Game);

	//if(!game || !game->Initialize())
		//return nullptr;

	return game;

}

#pragma endregion