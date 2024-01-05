// named mutex
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

HANDLE hMutex;
DWORD dwWaitResult;

void ProcessBaseCriticalSection() {
	dwWaitResult = WaitForSingleObject(hMutex, INFINITE);

	switch (dwWaitResult) {
	case WAIT_OBJECT_0:
		_fputts(_T("The thread is owning the Mutex.\n"), stdout);
		break;
	case WAIT_TIMEOUT:
		_fputts(_T("Timer got expired.\n"), stdout);
		break;
	case WAIT_ABANDONED:
		_fputts(_T("Mutex return didn't succeeded properly.\n"), stdout);
		break;
	}

	for (DWORD index = 0; index < 5; index++) {
		_tprintf(_T("Thread #%d is now running...\n"), index);
		Sleep(10000);
	}
	ReleaseMutex(hMutex);
}

int _tmain(int argc, TCHAR* argv[]) {
// set this precompiler as 0 and build as OpenMutex.exe.
// set this precompiler as 1 and build as CreateMutex.exe.
// run CreateMutex.exe first then OpenMutex.exe. CreateMutex.exe will perform its action and
// the execution flow will be automatically tossed to OpenMutex.exe because OpenMutex.exe is
// trying to access to the kernel objects for threads via named Mutex.
#if 1
	hMutex = CreateMutex(NULL, FALSE, _T("NamedMutex"));
#else
	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("NamedMutex"));
#endif
	if (hMutex == NULL) {
		_tprintf(_T("CreateMutex got an error, obtained an error code: %d\n"), GetLastError());
		return -1;
	}
	ProcessBaseCriticalSection();
	CloseHandle(hMutex);
	
	return 0;
}