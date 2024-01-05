// Testing about the critical section
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define	maximumThreadQty	6

LONG totalCount = 0;
HANDLE hMutex;

// Custom wrapping of acquiring mutex kernel object (recommended in practice)
DWORD AcquireMutex(HANDLE mutex) {
	return WaitForSingleObject(mutex, INFINITE);
}

// Hitting critical section
void increaseTotalCount() {
	AcquireMutex(hMutex);					// Wait until someone else using the mutex object release (custom wrapping)
	totalCount++;
	ReleaseMutex(hMutex);					// Before exiting the critical section, release the mutex kernel object
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

	// Create MuTex(Mutual Exclusion)
	hMutex = CreateMutex(
		NULL,		// default security attribute
		FALSE,		// everyone can acquire mutex (first-come-first-serve)
					// At first, this mutex object is not belonged to any other thread, created as "signaled" as a kernel object
		NULL		// nonamed mutex
	);

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

	// Delete mutex kernel object
	CloseHandle(hMutex);

	return 0;
}