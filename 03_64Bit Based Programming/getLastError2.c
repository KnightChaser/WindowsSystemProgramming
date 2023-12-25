// Error state change
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hFile = CreateFile(
		_T("thisWontExist.dat"),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL
	);
	
	// error code 2 will be raised because it doesn't exist
	if (hFile == INVALID_HANDLE_VALUE)
		_tprintf(_T("current error code: %d\n"), GetLastError());

	hFile = CreateFile(
		_T("thisWillBeCreated.dat"),
		GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL
	);

	// error code 0 will be set because the file is created and no error triggered
	// If I run this code more than 2 times consecutive, the error code 80 will be raised
	// because the target file is already created
    // GetLastError() will only capture the latest occurred error!
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD lastRaisedError = GetLastError();
		if (lastRaisedError == ERROR_FILE_EXISTS)
			_tprintf(_T("The file already exists"));
		else
			_tprintf(_T("An error code raised: %d\n"), lastRaisedError);
	}
	else {
		_tprintf(_T("the operation successfully accomplished without any error!\n"));
	}

	return 0;
}