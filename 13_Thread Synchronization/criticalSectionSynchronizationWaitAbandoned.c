// Semaphore's MUTEX_WAIT_ABANDONED
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

LONG gTotalCount = 0;
HANDLE hMutex;

DWORD AcquireMutex(HANDLE hMutex) {
	DWORD waitObjectResult = WaitForSingleObject(hMutex, INFINITE);
	return waitObjectResult;
}

UINT WINAPI IncreaseCountOne(LPVOID lpParam) {
	AcquireMutex(hMutex);
	gTotalCount++;
	// This **rude** thread does not release mutex object.
	return 0;
}

UINT WINAPI IncreaseCountTwo(LPVOID lpParam) {
	DWORD dwWaitResult = 0;
	// Expecting Mutex release
	dwWaitResult = AcquireMutex(hMutex);
	switch (dwWaitResult) {
	case WAIT_OBJECT_0:
		ReleaseMutex(hMutex);
		break;
	case WAIT_ABANDONED:
		_fputts(_T("Wait abandoned.\n"), stdout);
		break;
	}
	gTotalCount++;
	ReleaseMutex(hMutex);
	return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwThreadID1;	HANDLE hThread1;
	DWORD dwThreadID2;	HANDLE hThread2;
	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hMutex == NULL)
		_tprintf(_T("CreateMutex Error: %d\n"), GetLastError());

	// A rude thread that doesn't release semaphore properly! >:(
	hThread1 = (HANDLE)_beginthreadex(
		NULL, 0, IncreaseCountOne, NULL,
		0, (unsigned *)&dwThreadID1);

	// A normal thread!
	hThread2 = (HANDLE)_beginthreadex(
		NULL, 0, IncreaseCountTwo, NULL,
		CREATE_SUSPENDED, (unsigned*)&dwThreadID2);

	Sleep(1000);
	// Because hThread2 is suspended, hThread1 will be executed and it will cause not releaseing Mutex object problem.
	ResumeThread(hThread2);
	WaitForSingleObject(hThread2, INFINITE);
	_tprintf(_T("_gTotalCount value: %d\n"), gTotalCount);
	
	CloseHandle(hThread1);	// It didn't return the mutex... and just got terminated (suppose abnormal casess)
	CloseHandle(hThread2);
	CloseHandle(hMutex);
}