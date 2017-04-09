#include "USystem.h"

SystemData::SystemData()
	:systemType(SystemType::System_Invalid)
{

}

SystemData::SystemData(const SystemType &type)
	:systemType(type)
{

}

USystem::USystem(const SystemData &data)
	:UObject()
	, systemType(data.systemType)
{
	SetCanDraw(false);
}


USystem::~USystem(void)
{

}
