#ifndef _USYSTEM_H
#define _USYSTEM_H

#include "uobject.h"

//Additional includes

//Enum
enum SystemType
{
	System_Invalid,
	System_Game,
	System_Window,
	System_Input,
	System_Graphics,
	System_GameTimer,
	System_MAX
};

struct SystemData
{
	SystemData();
	SystemData(const SystemType &type);

	SystemType systemType;
};

class USystem :public UObject
{
	friend class Engine;
public:
	SystemType GetType() { return systemType;}

protected:
	USystem(const SystemData &data);
	virtual ~USystem(void);

	virtual bool Initialize() { return UObject::Initialize();}
	virtual bool Update(Context &context) { return UObject::Update(context);}
	virtual bool ShutDown() { return UObject::ShutDown();}

	SystemType systemType;

};

#endif