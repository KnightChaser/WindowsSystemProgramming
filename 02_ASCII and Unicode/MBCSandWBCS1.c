// (UNICODE)	: _T("str") => __T("str") => L"str" (WBCS)
// (NONUNICODE)	: _T("str") => __T("str") =>  "str" (MBCS)


#define UNICODE		// windows.h
#define _UNICODE	// tchar.h
// #undef UNICODE
// #undef _UNICODE

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int wmain(int argc, wchar_t argv[]) {
	TCHAR str[] = _T("Hello Hello Happiness");
	size_t size = sizeof(str);		// count up with NULL terminator
	size_t length = wcslen(str);	// count up without NULL terminator
	wprintf(L"string: %s\n", str);
	wprintf(L"string size: %d\n", size);
	wprintf(L"string length: %d\n", length);
	return 0;
}