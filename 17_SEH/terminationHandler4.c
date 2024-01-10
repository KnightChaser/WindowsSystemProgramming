// An exit handler about MUTEX release
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

#define maxThreadQty 10

LONG gTotalCount = 0;
HANDLE hMutex;

void increaseCount() {
	__try {
		WaitForSingleObject(hMutex, INFINITE);
		gTotalCount++;
	}
	__finally {
		ReleaseMutex(hMutex);
	}
}

UINT WINAPI threadFunction(LPVOID lpParam) {
	for (DWORD index = 0; index < 1000; index++)
		increaseCount();
	return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwThreadIDs[maxThreadQty];
	HANDLE hThreads[maxThreadQty];
	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hMutex == NULL)
		_tprintf(_T("CreateMutex() error, obtained an error code %d\n"), GetLastError());

	for (DWORD index = 0; index < maxThreadQty; index++) {
		hThreads[index] =
			(HANDLE)_beginthreadex(
				NULL, 0, threadFunction, NULL,
				CREATE_SUSPENDED, (unsigned*)&dwThreadIDs[index]);
		if (hThreads[index] == NULL) {
			_fputts(_T("Thread creation fault!\n"), stdout);
			return -1;
		}
	}

	for (DWORD index = 0; index < maxThreadQty; index++)
		ResumeThread(hThreads[index]);

	WaitForMultipleObjects(maxThreadQty, hThreads, TRUE, INFINITE);

	_tprintf(_T("Value of var gTotalCount: %d\n"), gTotalCount);

	for (DWORD index = 0; index < maxThreadQty; index++)
		CloseHandle(hThreads[index]);

	CloseHandle(hMutex);
	return 0;
}