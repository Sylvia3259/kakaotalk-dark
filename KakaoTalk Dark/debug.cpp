#include "pch.h"

#define MAX_COUNT 512

DWORD WriteLogA(LPCSTR lpszFormat, ...) {
	CHAR szLog[MAX_COUNT];
	DWORD dwCharsWritten;

	va_list args;
	va_start(args, lpszFormat);
	vsprintf_s(szLog, MAX_COUNT, lpszFormat, args);
	va_end(args);

	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), szLog, strlen(szLog), &dwCharsWritten, NULL);

	return dwCharsWritten;
}

DWORD WriteLogW(LPCWSTR lpszFormat, ...) {
	WCHAR szLog[MAX_COUNT];
	DWORD dwCharsWritten;

	va_list args;
	va_start(args, lpszFormat);
	vswprintf_s(szLog, MAX_COUNT, lpszFormat, args);
	va_end(args);

	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), szLog, wcslen(szLog), &dwCharsWritten, NULL);

	return dwCharsWritten;
}