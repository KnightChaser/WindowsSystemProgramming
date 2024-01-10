// Observation of exit handler
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	TCHAR str[2];
	__try {
		// buffer overflow (exception)
		_tcscpy(str, _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
		_tprintf(_T("%s\n"), str);
	}
	__finally {
		_fputts(_T("__finally block\n"), stdout);
	}

	return 0;
}