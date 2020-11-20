#include "pch.h"
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

typedef HBRUSH(WINAPI* ORGFP1) (COLORREF);
typedef COLORREF(WINAPI* ORGFP2) (HDC, COLORREF);
typedef GpStatus(WINGDIPAPI* ORGFP3) (GDIPCONST WCHAR* filename, GpBitmap** bitmap);
typedef GpStatus(WINGDIPAPI* ORGFP4) (GDIPCONST WCHAR* filename, GpBitmap** bitmap);
typedef GpStatus(WINGDIPAPI* ORGFP5) (IStream* stream, GpBitmap** bitmap);
typedef GpStatus(WINGDIPAPI* ORGFP6) (IStream* stream, GpBitmap** bitmap);
ORGFP1 originFunc1 = CreateSolidBrush;
ORGFP2 originFunc2 = SetTextColor;
ORGFP3 originFunc3 = GdipCreateBitmapFromFile;
ORGFP4 originFunc4 = GdipCreateBitmapFromFileICM;
ORGFP5 originFunc5 = GdipCreateBitmapFromStream;
ORGFP6 originFunc6 = GdipCreateBitmapFromStreamICM;

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
	WriteLog(TEXT("Background : (%d, %d, %d)\n"), (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
	color = 0xFFFFFF - color;
	return originFunc1(color);
}

COLORREF WINAPI DetourFunc2(HDC hdc, COLORREF color) {
	WriteLog(TEXT("Text : (%d, %d, %d)\n"), (color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
	color = 0xFFFFFF - color;
	return originFunc2(hdc, color);
}

GpStatus WINGDIPAPI DetourFunc3(GDIPCONST WCHAR* filename, GpBitmap** bitmap) {
	WriteLog(TEXT("GdipCreateBitmapFromFile(%s, 0x%08X)\n"), filename, bitmap);
	return originFunc3(filename, bitmap);
}

GpStatus WINGDIPAPI DetourFunc4(GDIPCONST WCHAR* filename, GpBitmap** bitmap) {
	WriteLog(TEXT("GdipCreateBitmapFromFileICM(%s, 0x%08X)\n"), filename, bitmap);
	return originFunc4(filename, bitmap);
}

GpStatus WINGDIPAPI DetourFunc5(IStream* stream, GpBitmap** bitmap) {
	WriteLog(TEXT("GdipCreateBitmapFromStream(0x%08X, 0x%08X)\n"), stream, bitmap);
	return originFunc5(stream, bitmap);
}

GpStatus WINGDIPAPI DetourFunc6(IStream* stream, GpBitmap** bitmap) {
	WriteLog(TEXT("GdipCreateBitmapFromStreamICM(0x%08X, 0x%08X)\n"), stream, bitmap);
	return originFunc6(stream, bitmap);
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
		DetourAttach(&(PVOID&)originFunc3, DetourFunc3);
		DetourAttach(&(PVOID&)originFunc4, DetourFunc4);
		DetourAttach(&(PVOID&)originFunc5, DetourFunc5);
		DetourAttach(&(PVOID&)originFunc6, DetourFunc6);
		DetourTransactionCommit();
		break;
	case DLL_PROCESS_DETACH:
		FreeConsole();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)originFunc1, DetourFunc1);
		DetourDetach(&(PVOID&)originFunc2, DetourFunc2);
		DetourDetach(&(PVOID&)originFunc3, DetourFunc3);
		DetourDetach(&(PVOID&)originFunc4, DetourFunc4);
		DetourDetach(&(PVOID&)originFunc5, DetourFunc5);
		DetourDetach(&(PVOID&)originFunc6, DetourFunc6);
		DetourTransactionCommit();
		break;
	}

	return TRUE;
}