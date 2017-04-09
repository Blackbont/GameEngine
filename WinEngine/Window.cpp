#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window(const WindowData &data)
	:USystem(data)
	, hWindow(NULL)
	, hDC(NULL)
	, hInstance(NULL)
	, height(data.height)
	, width(data.width)
	, bits(data.bits)
	, isFullScreen(data.isFullScreen)
	, title(data.title)
	, resizeData()
{

}


Window::~Window(void)
{
}

bool Window::Initialize()
{
	USystem::Initialize();

	unsigned int pixelFormat = 0;

	DWORD dwExStyle, dwStyle;

	RECT windowRect;
	windowRect.left = 0;
	windowRect.right = width;
	windowRect.top = 0;
	windowRect.bottom = height;

	WNDCLASS windowClass;	
	windowClass.lpszClassName = title.c_str();
	windowClass.style = CS_DBLCLKS | CS_DROPSHADOW | CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)WindowProc;
	windowClass.cbClsExtra = NULL;
	windowClass.cbWndExtra = NULL;
	windowClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;

	if(!RegisterClass(&windowClass))
	{
		//Logger::Log(_T("Failed to register window!"), LOGTYPE_ERROR, true);
		return false;
	}

	if(isFullScreen)
	{
		DEVMODE ScreenSettings;

		memset(&ScreenSettings, 0, sizeof(ScreenSettings) );
		ScreenSettings.dmSize       = sizeof(ScreenSettings);
		ScreenSettings.dmPelsWidth  = width;
		ScreenSettings.dmPelsHeight = height;
		ScreenSettings.dmBitsPerPel = bits;
		ScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&ScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
		{
			//Logger::Log(_T("The Requested fullscreen mode is not supported by your video card!"), LOGTYPE_ERROR, true);

			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			//ShowCursor(FALSE);
		}
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
		//ShowCursor(TRUE);
	}

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	if(!(hWindow = CreateWindowEx(dwExStyle,
		title.c_str(),
		title.c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		this)))
	{
		//Logger::Log(_T("Failed to window handle = NULL"), LOGTYPE_ERROR, true);
		return false;
	}

	static  PIXELFORMATDESCRIPTOR pfd =  // Cообщает Windows каким будет вывод на экран каждого пикселя
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		bits,               // Глубина цвета
	 //[R][RS][G][GS][B][BS]
		0, 0,  0, 0,  0, 0, // Цветовые биты которые будут проигнорированы [Red, RedShift, Green, GreenShift, Blue, BlueShift]
	 //[A][AS]
		0, 0,               // Alpha bits [Alpha, AlphaShift]
		0,                  // AccumBits 
		0, 0, 0, 0,			// [Accum Red, Green, Blue, Alpha bits] 
		16,                 // Z-буфер (буфер глубины)
		0,					// Stencil buffer
		0,					// Aux buffer
		PFD_MAIN_PLANE,     // Главный слой рисования
		0,					// Число наложений
		0, 0, 0				// [Not used, Visible mask - Тот цвет который будет прозрачным, Not used]
	};

	if(!(hDC = GetDC(hWindow)))
	{
		//Logger::Log(_T("Can't create device context"), LOGTYPE_ERROR, true);
		return false;
	}
	if(!(pixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		//Logger::Log(_T("Can't choose pixel format"), LOGTYPE_ERROR, true);
		return false;
	}
	if(!SetPixelFormat(hDC, pixelFormat, &pfd ) )
	{
		//Logger::Log(_T("Can't set the pixel format"), LOGTYPE_ERROR, true);
		return false;
	}

	ShowWindow(hWindow, SW_SHOW);
	SetForegroundWindow(hWindow);
	SetFocus(hWindow);
	UpdateWindow(hWindow);

	resizeData.isMustResize = true;
	resizeData.newHeight = height;
	resizeData.newWidth = width;

	if(!this->CenterWindow())
	{
		//Logger::Log(_T("Failed to center window"), LOGTYPE_ERROR, true);
		return false;
	}

	//Убирает в консольке кнопку "Закрыть", не позволяя консоли закрыться после закрытия основного окна
	HWND consoleWindow = GetConsoleWindow();
	if(consoleWindow){
		HMENU consoleMenu = GetSystemMenu(consoleWindow, FALSE);
		if(consoleMenu){
			BOOL rem = RemoveMenu(consoleMenu, SC_CLOSE, MF_BYCOMMAND);
			if(!rem){
				//Logger::Log(_T("Can't remove menu button"), LOGTYPE_ERROR, true);
				return false;
			}
		}
	}

	//Logger::Log(_T("Window is initialized"), LOGTYPE_INFO, true);

	//Ура! Оно работает
	return true;
}

bool Window::Update(Context &context)
{
	if(context.hWnd != this)
		context.hWnd = this;
	
	return true;
}

bool Window::ShutDown()
{
	if(!isFullScreen){
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if(hDC && !ReleaseDC(hWindow, hDC)){
		//Logger::Log(_T("Fail release DC"), LOGTYPE_ERROR, true);
		hDC = NULL;
		return false;
	}
	if(hWindow && !DestroyWindow(hWindow)){
		//Logger::Log(_T("Fail destroy window"), LOGTYPE_ERROR, true);
		hWindow = NULL;
		return false;
	}
	if(!UnregisterClass(title.c_str(), hInstance )){
		//Logger::Log(_T("Fail unregister class"), LOGTYPE_ERROR, true);
		hInstance = NULL;
		return false;
	}
	return true;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if(msg == WM_CREATE){
		CREATESTRUCT *CS = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)CS->lpCreateParams);
	}
	else{
		Window *window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(window) 
			return window->HandleEvent(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_ACTIVATE:
		{
			if(!HIWORD(wParam)) this->Activate();
				else this->DeActivate();
			return 0;
		}
		case WM_SIZE:
		{
			UINT tempWidth = LOWORD(wParam);
			UINT tempHeight = HIWORD(wParam);

			resizeData.isMustResize = true;
			resizeData.newHeight = true;
			resizeData.newWidth = true;
			return 0;
		}
		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


bool Window::CenterWindow()
{
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle =   WS_OVERLAPPEDWINDOW;
	RECT rect = {0, 0, width, height};

	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
	
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	/*int windowWidth = width + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
 	int windowHeight = height + GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	if(windowClass.lpszMenuName != NULL) windowHeight += GetSystemMetrics(SM_CYMENU);*/
	/*
	int windowPosX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
	int windowPosY = (GetSystemMetrics(SM_CXSCREEN) - windowHeight) / 2;*/

	int windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	int windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;

	if(!SetWindowPos(hWindow, NULL, windowPosX, windowPosY, windowWidth, windowHeight, SWP_SHOWWINDOW))
		return false;


	return true;
}

bool Window::SetPosition(int x, int y)
{
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle =   WS_OVERLAPPEDWINDOW;
	RECT rect = {0, 0, width, height};

	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	if(!SetWindowPos(hWindow, NULL, x, y, windowWidth, windowHeight, SWP_SHOWWINDOW))
		return false;

	return true;
}

WindowData::WindowData()
	: SystemData(SystemType::System_Invalid)
	, width(-1)
	, height(-1)
	, bits(-1)
	, title(_T(""))
	, isFullScreen(false)
{

}
WindowData::WindowData(int width, int height, const std::tstring &title /*= _T("NoNameEngine V 1.0")*/, int bits /*= 32*/, bool isFullScreen /*= false*/)
	: SystemData(SystemType::System_Window)
	, width(width)
	, height(height)
	, bits(bits)
	, isFullScreen(isFullScreen)
	, title(title)
{

}

ResizeData::ResizeData()
	: isMustResize(false)
	, newWidth(-1)
	, newHeight(-1)
{

}
ResizeData::ResizeData(bool isResize, int newWidth, int newHeight)
	: isMustResize(isResize)
	, newWidth(newWidth)
	, newHeight(newHeight)
{

}
