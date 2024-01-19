// Unicode-based file content printing
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");
	TCHAR filedata[] = _T("Just test string for File I/O...");

	HANDLE hFile = CreateFile(
		filename,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD dwError = GetLastError();
		LPVOID errorMessage;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dwError,
			0,
			(LPTSTR)&errorMessage,
			0,
			NULL
		);
		_tprintf(_T("File creation fault: %s\n"), errorMessage);
		LocalFree(errorMessage);
		return -1;
	}

	DWORD numberOfByteWritten = 0;
	WriteFile(
		hFile,
		filedata,
		_tcslen(filedata) * sizeof(TCHAR),
		&numberOfByteWritten,
		NULL);

	_fputts(_T("Written data size: %d bytes\n"), numberOfByteWritten);
	CloseHandle(hFile);
	
	return 0;
}