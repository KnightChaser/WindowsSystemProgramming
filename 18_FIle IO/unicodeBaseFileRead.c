#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LENGTH_BYTE	100

int _tmain(int argc, TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");
	TCHAR filedata[STRING_LENGTH_BYTE];

	HANDLE hFile = CreateFile(
		filename, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		_fputts(_T("File open fault\n"), stdout);
		return -1;
	}

	DWORD numberOfByteRead = 0;
	ReadFile(hFile, filedata, (size_t)(_tcslen(filedata) * sizeof(TCHAR)), &numberOfByteRead, NULL);
	filedata[numberOfByteRead / sizeof(TCHAR)] = _T('\0');

	_tprintf(_T("Read data size: %u bytes\n"), numberOfByteRead);
	_tprintf(_T("Read string: %s\n"), filedata);
	CloseHandle(hFile);

	return 0;
}