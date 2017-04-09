#ifndef _WINDOW_H
#define _WINDOW_H

#include "usystem.h"

//C Runtime headers
#ifndef _WINDOWS_
	#include <Windows.h>
#endif

//Additional includes


struct WindowData : public  SystemData
{
	WindowData();
	WindowData(int width, int height, const std::tstring &title = _T("NoNameEngine V 1.0"), int bits = 32, bool isFullScreen = false);

	int width, height;
	std::tstring title;
	int bits;
	bool isFullScreen;
};

struct ResizeData
{
	ResizeData();
	ResizeData(bool isResize, int newWidth, int newHeight);

	bool isMustResize;
	int newWidth, newHeight;
};

class Window : public USystem
{
	friend class Engine;
public:
	HWND GetWindowHandle() {return hWindow;}
	HDC GetWindowDC() {return hDC;}
	HINSTANCE GetInstance() {return hInstance;}

	LRESULT HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int GetWidth() {return windowData.width;}
	int GetHeight() {return windowData.height;}

	ResizeData &GetResizeData() {return resizeData;}
	
	bool CenterWindow();
	bool SetPosition(int x, int y);
protected:
	Window(const WindowData &windowData);
	virtual ~Window(void);

	virtual bool Initialize();
	virtual bool Update(Context &context);
	virtual bool ShutDown();

private:

	//Members
	WindowData windowData;
	ResizeData resizeData;

	HWND hWindow;
	HDC hDC;
	HINSTANCE hInstance;

	int width, height;
	std::tstring title;
	int bits;
	bool isFullScreen;
};

#endif