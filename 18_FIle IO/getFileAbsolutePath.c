// Getting full path(absolute path) with the file name
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LENGTH_BYTE	100

int _tmain(int argc, _TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");
	TCHAR fileFullPathName[STRING_LENGTH_BYTE];
	LPTSTR filePtr;

	GetFullPathName(filename, STRING_LENGTH_BYTE, fileFullPathName, &filePtr);

	_tprintf(_T("%s \n"), fileFullPathName);	// Aboluste path also includes the target file name itself.
	_tprintf(_T("%s \n"), filePtr);

	return 0;
}