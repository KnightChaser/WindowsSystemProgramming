// Testing about the critical section
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define	maximumThreadQty	6

LONG totalCount = 0;
CRITICAL_SECTION hCriticalSection;	// A key to control the specific critical section(IncreaseTotalCount() for here)

// Hitting critical section
void increaseTotalCount() {
	EnterCriticalSection(&hCriticalSection);	// Enter the critical section, lock the key
	totalCount++;
	LeaveCriticalSection(&hCriticalSection);	// Leave the critical section, relase the key
}

// Thread trying to call the problematic function about the critical section
UINT WINAPI ThreadFunction(LPVOID lpParam) {
	for (DWORD i = 0; i < 10000; i++)
		increaseTotalCount();

	return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwThreadId[maximumThreadQty];
	HANDLE hThread[maximumThreadQty];

	InitializeCriticalSection(&hCriticalSection);	// Initialize the critical management section resource
	
	for (DWORD index = 0; index < maximumThreadQty; index++) {
		hThread[index] = (HANDLE)_beginthreadex(
			NULL, 0, ThreadFunction, NULL,
			CREATE_SUSPENDED, (unsigned*)&dwThreadId[index]
		);
		if (hThread[index] == NULL) {
			_tprintf(_T("Thread creation fault at index %d\n"), index);
			return -1;
		}
	}

	for (DWORD index = 0; index < maximumThreadQty; index++)
		ResumeThread(hThread[index]);

	// Wait until the given threads finish working
	WaitForMultipleObjects(maximumThreadQty, hThread, TRUE, INFINITE);

	_tprintf(_T("total count: %d\n"), totalCount);

	// Closing the used threads' handles
	for (DWORD index = 0; index < maximumThreadQty; index++)
		CloseHandle(hThread[index]);

	DeleteCriticalSection(&hCriticalSection);	// Free the critical section management resource
	return 0;
}