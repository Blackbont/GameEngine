#ifndef _DELETEMACROS_H
#define _DELETEMACROS_H

//Defines
#define SafeRelease(x) {if(x) {x->Release(); x = nullptr;} }// ������ ����������� �������� COM �������
#define SafeDelete(x) {if(x) {delete x; x = nullptr;} }// ������ ����������� �������� �������

#endif