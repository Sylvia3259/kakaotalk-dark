#include "pch.h"
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

COLORREF(WINAPI* lpfnSetTextColor)(HDC, COLORREF) = SetTextColor;
HBRUSH(WINAPI* lpfnCreateSolidBrush)(COLORREF) = CreateSolidBrush;
GpStatus(WINGDIPAPI* lpfnGdipCreateBitmapFromFile)(GDIPCONST WCHAR*, GpBitmap**) = GdipCreateBitmapFromFile;

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

COLORREF WINAPI MySetTextColor(HDC hdc, COLORREF color) {
	WriteLog(TEXT("SetTextColor(0x%08X, RGB(%d, %d, %d))\n"), hdc, GetRValue(color), GetGValue(color), GetBValue(color));
	color = 0xFFFFFF - color;
	return lpfnSetTextColor(hdc, color);
}

HBRUSH WINAPI MyCreateSolidBrush(COLORREF color) {
	WriteLog(TEXT("CreateSolidBrush(RGB(%d, %d, %d))\n"), GetRValue(color), GetGValue(color), GetBValue(color));
	color = 0xFFFFFF - color;
	return lpfnCreateSolidBrush(color);
}

GpStatus WINGDIPAPI MyGdipCreateBitmapFromFile(GDIPCONST WCHAR* filename, GpBitmap** bitmap) {
	WriteLog(TEXT("GdipCreateBitmapFromFile(\"%s\", 0x%08X)\n"), filename, bitmap);
	return lpfnGdipCreateBitmapFromFile(filename, bitmap);
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
		DetourAttach(&(PVOID&)lpfnSetTextColor, MySetTextColor);
		DetourAttach(&(PVOID&)lpfnCreateSolidBrush, MyCreateSolidBrush);
		DetourAttach(&(PVOID&)lpfnGdipCreateBitmapFromFile, MyGdipCreateBitmapFromFile);
		DetourTransactionCommit();
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)lpfnSetTextColor, MySetTextColor);
		DetourDetach(&(PVOID&)lpfnCreateSolidBrush, MyCreateSolidBrush);
		DetourDetach(&(PVOID&)lpfnGdipCreateBitmapFromFile, MyGdipCreateBitmapFromFile);
		DetourTransactionCommit();
		break;
	}

	return TRUE;
}