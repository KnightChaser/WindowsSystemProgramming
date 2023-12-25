// Getting error cause in Windows native environment

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hFile = CreateFile(
		_T("owo.txt"),
			GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL
		);

	// It will raise an error code 2
	// that means the system couldn't find the file specified.
	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(_T("error code: %d\n"), GetLastError());
		return -1;
	}
	return 0;
}