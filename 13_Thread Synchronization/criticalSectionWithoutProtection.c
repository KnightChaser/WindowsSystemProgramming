// Testing about the critical section
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define	maximumThreadQty	6

LONG totalCount = 0;

// Hitting critical section
void increaseTotalCount() {
	totalCount++;
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

	return 0;
}