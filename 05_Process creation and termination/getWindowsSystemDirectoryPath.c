#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define DIR_LEN MAX_PATH + 1

int _tmain(int argc, TCHAR** argv) {
	TCHAR systemDirectory[DIR_LEN];
	TCHAR windowsDefaultDirectory[DIR_LEN];

	// Extract the system directory information (e.g. C:\WINDOWS\system32)
	GetSystemDirectory(systemDirectory, DIR_LEN);

	// Extract Windows directory information (E.G. C:\WINDOWS)
	GetWindowsDirectory(windowsDefaultDirectory, DIR_LEN);

	_tprintf(_T("system direcotyr: %s\n"), systemDirectory);
	_tprintf(_T("windows default directory: %s\n"), windowsDefaultDirectory);

	return 0;
}