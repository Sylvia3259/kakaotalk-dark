#pragma once

DWORD WriteLogA(LPCSTR lpszFormat, ...); 
DWORD WriteLogW(LPCWSTR lpszFormat, ...);

#ifdef UNICODE
#define WriteLog  WriteLogW
#else
#define WriteLog  WriteLogA
#endif