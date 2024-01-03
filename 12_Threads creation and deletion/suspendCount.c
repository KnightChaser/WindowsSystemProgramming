// suspendCount.c
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

UINT WINAPI ThreadFunction(LPVOID lpParam) {
	while (TRUE) {
		_fputts(_T("Running state\n"), stdout);
		Sleep(1000);
	}
	return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD	dwThreadId;
	HANDLE	hThread;
	DWORD	suspendCount;		// suspend count from SuspendThread() and ResumeThread()

	// Create a thread of ThreadFunction() with CREATE_SUSPENDED flag, starting suspend count with 1.
	hThread = (HANDLE)_beginthreadex(
		NULL, 0, ThreadFunction, NULL,
		CREATE_SUSPENDED, (unsigned*)&dwThreadId);

	// Check if the thread created
	if (hThread == NULL) {
		_tprintf(_T("Thread creation fault, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	// suspend count gets +1 per each SuspendCount() call
	// suspend count gets -1 per each ResumeThread() call
	suspendCount = ResumeThread(hThread);
	_tprintf(_T("suspend count: %d\n"), suspendCount);
	Sleep(1000);

	suspendCount = SuspendThread(hThread);
	_tprintf(_T("suspend count: %d\n"), suspendCount);
	Sleep(1000);

	suspendCount = SuspendThread(hThread);
	_tprintf(_T("suspend count: %d\n"), suspendCount);
	Sleep(1000);

	suspendCount = ResumeThread(hThread);
	_tprintf(_T("suspend count: %d\n"), suspendCount);
	Sleep(1000);

	suspendCount = ResumeThread(hThread);
	_tprintf(_T("suspend count: %d\n"), suspendCount);
	Sleep(1000);

	WaitForSingleObject(hThread, INFINITE);

	return 0;
}