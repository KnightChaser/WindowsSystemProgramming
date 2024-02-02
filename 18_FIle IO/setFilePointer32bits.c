// File pointer modification in 32 bit environment
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LENGTH_BYTE 100

TCHAR fileData[] = _T("abcdefghijklmnopqrstuvwxyz");

int _tmain(int argc, _TCHAR* argv[]) {
	TCHAR fileName[] = _T("data.txT");
	TCHAR readBuffer[STRING_LENGTH_BYTE];
	HANDLE hFile;
	DWORD numberOfByteWritten = 0;
	DWORD dwPtr = 0;

	// file write
	hFile = CreateFile(fileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	WriteFile(hFile, fileData, _tcslen(fileData) * sizeof(TCHAR), &numberOfByteWritten, NULL);
	CloseHandle(hFile);

	// file read
	hFile = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	ReadFile(hFile, readBuffer, sizeof(readBuffer), &numberOfByteWritten, NULL);
	_tprintf(_T("%s \n"), readBuffer);

	// Move the file pointer to the first byte
	dwPtr = SetFilePointer(hFile, sizeof(TCHAR) * 4, NULL, FILE_BEGIN);		// Move the file pointer to 4 * TCHAR since FILE_BEGIN
	if (dwPtr == INVALID_SET_FILE_POINTER) {
		_tprintf(_T("SetFIlePointer error, got an obtained error code: %d\n"), GetLastError());
		return -1;
	}
	ReadFile(hFile, readBuffer, sizeof(readBuffer), &numberOfByteWritten, NULL);
	_tprintf(_T("%s \n"), readBuffer);
	
	// Move the file pointer to the last
	dwPtr = SetFilePointer(hFile, sizeof(TCHAR) * 4, NULL, FILE_END);		// Move the file pointer to 4 * TCHAR from FILE_END
	if (dwPtr == INVALID_SET_FILE_POINTER) {
		_tprintf(_T("SetFIlePointer error, got an obtained error code: %d\n"), GetLastError());
		return -1;
	}
	ReadFile(hFile, readBuffer, sizeof(readBuffer), &numberOfByteWritten, NULL);

	CloseHandle(hFile);
	return 0;
}