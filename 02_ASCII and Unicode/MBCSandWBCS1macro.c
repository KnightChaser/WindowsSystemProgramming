// (UNICODE)	: _T("str") => __T("str") => L"str" (WBCS)
// (NONUNICODE)	: _T("str") => __T("str") =>  "str" (MBCS)


#define UNICODE     // windows.h
#define _UNICODE    // tchar.h
//#undef UNICODE
//#undef _UNICODE

#ifdef _UNICODE
    #define _tprintf wprintf
    #define _tstrlen wcslen
#else
    #define _tprintf printf
    #define _tstrlen strlen
#endif

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int wmain(int argc, wchar_t* argv[]) {
    TCHAR str[] = _T("Hello Hello Happiness");
    size_t size = sizeof(str);             // count up with NULL terminator
    size_t length = _tstrlen(str);         // count up without NULL terminator
    
    _tprintf(_T("string: %s\n"), str);
    _tprintf(_T("string size: %d\n"), size);
    _tprintf(_T("string length: %d\n"), length);

    return 0;
}