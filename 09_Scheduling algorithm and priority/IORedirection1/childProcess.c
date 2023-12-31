// example.c
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[]) {
    _fputts(_T("Hello World!\n"), stdout);
    return 0;
}