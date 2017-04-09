#ifndef _DELETEMACROS_H
#define _DELETEMACROS_H

//Defines
#define SafeRelease(x) {if(x) {x->Release(); x = nullptr;} }// Макрос безопасного удаления COM обьекта
#define SafeDelete(x) {if(x) {delete x; x = nullptr;} }// Макрос безопасного удаления обьекта

#endif