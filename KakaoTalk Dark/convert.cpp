#include "pch.h"

inline static BOOL IsFileExist(LPCWSTR name) {
	FILE* pFile = NULL;
	errno_t err = _wfopen_s(&pFile, name, L"r");
	if (pFile) fclose(pFile);
	return !err;
}

void ConvertImageResourceTheme(LPCWSTR lpszOriginalFilename, LPWSTR lpszConvertedFilename, DWORD dwSize) {
	const WCHAR szOriginalBasePath[] = L"C:\\Program Files (x86)\\Kakao\\KakaoTalk\\skin\\default\\image\\";
	const WCHAR szConvertedBasePath[] = L"C:\\KakaoTalk Dark Test\\skin\\default\\image\\";

	if (!lpszOriginalFilename || !lpszConvertedFilename) return;

	if (wcsstr(lpszOriginalFilename, szOriginalBasePath) == lpszOriginalFilename) {
		swprintf_s(lpszConvertedFilename, dwSize, L"%s%s", szConvertedBasePath, lpszOriginalFilename + wcslen(szOriginalBasePath));
		if (IsFileExist(lpszConvertedFilename)) return;
	}
	wcscpy_s(lpszConvertedFilename, dwSize, lpszOriginalFilename);
}