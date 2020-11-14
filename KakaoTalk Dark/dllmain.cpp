#include "pch.h"

typedef HBRUSH(WINAPI* ORGFP1) (COLORREF);
typedef COLORREF(WINAPI* ORGFP2) (HDC, COLORREF);
//typedef COLORREF(WINAPI* ORGFP3) (HDC, COLORREF);
//typedef int(WINAPI* ORGFP4) (HDC, int);
typedef HANDLE(WINAPI* ORGFP3) (HINSTANCE, LPCWSTR, UINT, int, int, UINT);
//typedef BOOL(WINAPI* ORGFP3) (int, const INT*, const COLORREF*);
ORGFP1 originFunc1 = CreateSolidBrush;
ORGFP2 originFunc2 = SetTextColor;
//ORGFP3 originFunc3 = SetBkColor;
//ORGFP4 originFunc4 = SetBkMode;
ORGFP3 originFunc3 = LoadImageW;

DWORD WriteLog(LPCTSTR lpszFormat, ...) {
	TCHAR szLog[512];
	DWORD dwCharsWritten;

	va_list args;
	va_start(args, lpszFormat);
	_vstprintf_s(szLog, 512, lpszFormat, args);
	va_end(args);

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szLog, _tcslen(szLog), &dwCharsWritten, NULL);

	return dwCharsWritten;
}

HBRUSH WINAPI DetourFunc1(COLORREF color) {
	color = 0x202020;
	return originFunc1(color);
}

COLORREF WINAPI DetourFunc2(HDC hdc, COLORREF color) {
	color = 0xCCCCCC;
	return originFunc2(hdc, color);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (DetourIsHelperProcess())
		return TRUE;

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)originFunc1, DetourFunc1);
		DetourAttach(&(PVOID&)originFunc2, DetourFunc2);
		DetourTransactionCommit();
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)originFunc1, DetourFunc1);
		DetourDetach(&(PVOID&)originFunc2, DetourFunc2);
		DetourTransactionCommit();
		break;
	}

	return TRUE;
}