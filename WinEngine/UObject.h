#ifndef _UOBJECT_H
#define _UOBJECT_H

#ifndef _CONTEXT_H
	#include "context.h"
#endif

#ifndef _STRING_H
	#include "string.h"
#endif

class UObject
{
public:
	UObject();
	UObject(const std::tstring &name);
	virtual ~UObject(void);

	//void* operator new(size_t size);
	//void operator delete(void* pointer);
	virtual bool Initialize() { isInitialized = true; return true;}
	virtual bool PostInitialize() { isPostInitializing = true; return true;}
	virtual bool LoadContent() { isContentLoaded = true; return true;}
	virtual bool PostLoadContent() { isPostContentLoaded = true; return true;}
	virtual bool Update(Context &context) {return true;}
	virtual bool Draw(Context &context) {return true;}
	virtual bool DrawUI(Context &context) {return true;}
	virtual bool ShutDown() {return true;}
	virtual void reset() {}

	void SetName(const std::tstring &name) { this->name = name;}
	const std::tstring &GetString() const { return name;}

	bool IsInitialized() const { return isInitialized;}
	bool IsPostInitialized() const { return isPostInitializing;}
	bool IsContentLoaded() const { return isContentLoaded;}
	bool IsPostContentLoaded() const { return isPostContentLoaded;}

	bool IsDestroyed() const { return isDestroyed;}
	bool IsCanTick() const { return isCanTick;}
	bool IsCanDraw() const { return isCanDraw;}

	void Destroy() { isDestroyed = true;}
	void Activate() { isActivated = true;}
	void DeActivate() { isActivated = false;}
	bool IsActivated() { return isActivated;}
	void SetCanTick(bool isCanTick) { this->isCanTick = isCanTick;}
	void SetDestroyed(bool isDestroyed) { this->isDestroyed = isDestroyed;}
	void SetCanDraw(bool isCanDraw) { this->isCanDraw = isCanDraw;}


protected:
	static int objectAmount;
	int ID;

	bool isInitialized;
	bool isPostInitializing;
	bool isContentLoaded;
	bool isPostContentLoaded;
	
private:
	std::tstring name;
	bool isDestroyed;
	bool isCanTick;
	bool isCanDraw;
	bool isActivated;
};

#endif