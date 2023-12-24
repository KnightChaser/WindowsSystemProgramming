// Let's fully understand MBCS and WBCS

#define _CRT_SECURE_NO_WARNINGS
#define UNICODE		// windows.h
#define _UNICODE	// tchar.h

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	
	LPCTSTR str1 = _T("MBCS or WBCS? OwO");
	TCHAR str2[] = _T("MBCS or WBCS? UwU");
	_tprintf(_T("str1: %s\n"), str1);
	_tprintf(_T("str2: %s\n"), str2);
	TCHAR str3[100];
	TCHAR str4[50];
	LPCTSTR pStr = str1;

	_tprintf(_T("string size: %d\n"), sizeof(str2));
	_tprintf(_T("stirng length: %d\n"), _tcslen(pStr));
	
	_fputts(_T("input string1(to str3): "), stdout);
	_tscanf(_T("%s"), str3);
	_fputts(_T("input string2(to str4): "), stdout);
	_tscanf(_T("%s"), str4);

	// concatenation
	_tcscat(str3, str4);
	_tprintf(_T("WCHAR str3 + WCHAR str4 = %s\n"), str3);

	return 0;

}