#include "UObject.h"

int UObject::objectAmount = 0;

UObject::UObject(const std::tstring &name)
	:name(name)
	, ID(objectAmount)
	, isInitialized(false)
	, isPostInitializing(false)
	, isContentLoaded(false)
	, isPostContentLoaded(false)
	, isDestroyed(false)

	, isCanTick(true)
	, isCanDraw(true)
{
	objectAmount++;

}

UObject::UObject()
	:name(_T(""))
	, ID(objectAmount)
	, isInitialized(false)
	, isPostInitializing(false)
	, isContentLoaded(false)
	, isPostContentLoaded(false)
	, isDestroyed(false)

	, isCanTick(true)
	, isCanDraw(true)
{
	name = _T("Object_") + TOSTRING((_ULonglong)ID);
	objectAmount++;
}


UObject::~UObject(void)
{
	objectAmount--;
}
