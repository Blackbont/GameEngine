#ifndef _IOSTREAM_H
#define _IOSTREAM_H

//C runtime headers
#ifndef _IOSTREAM_
	#include <iostream>
#endif

//Defines
#ifdef UNICODE  // Если нужна потдержка юникода
	#define tcout wcout
	#define tcin wcin
#else // Для мультибайт кодировки
	#define tcout cout
	#define tcin cin
#endif

#endif // _IOSTEAM_H