// DuplicateHandleChildProcess.c

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	// Receive the real process handle value from the parent process(DuplicateHandle.exe)
	HANDLE hParent = (HANDLE)_ttoi(argv[1]);
	DWORD isParentProcessSuccessful = WaitForSingleObject(hParent, INFINITE);

	_fputts(_T("[Child process]\n"), stdout);

	if (isParentProcessSuccessful == WAIT_FAILED) {
		_fputts(_T("WAIT_FAILED returned from the parent process(DuplicatedHandle.exe) returned!\n"), stdout);
		_gettchar();
		return -1;
	}
	else {
		_fputts(_T("Wait was successful."), stdout);
		_tprintf(_T("Handle received from the parent process: %d\n"), hParent);
		_gettchar();
		return 0;
	}

}