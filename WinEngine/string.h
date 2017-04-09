#ifndef _STRING_H
#define _STRING_H

//C runtime headers 
#include <tchar.h>
#include <string>

//Defines
#ifdef UNICODE // Если нужна потдержка юникода
	#define tstring wstring
	#define TOSTRING(value) std::to_wstring(value)
#else // Для мультибайт кодировки
	#define tstring string
	#define TOSTRING(value) std::to_string(value)
#endif

#endif //_STRING_H