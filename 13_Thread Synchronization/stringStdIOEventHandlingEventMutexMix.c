// understanding producer-consumer model
//  			 and synchronized events
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

UINT WINAPI outputThreadFunction(LPVOID lpParam);
UINT WINAPI countThreadFunction(LPVOID lpParam);

// event to signal the multiple threads at the expected moment(s),
// mutex to protect the shard variable(SyncEventMutexObj.string in this case).
// => This combination is often used in practice.
typedef struct _synchronizationEventMutexMixStruct {
	TCHAR string[100];
	HANDLE hEvent;			// event (signaled, unsigned)
	HANDLE hMutex;			// mutex (acquired, released)
} SynchEventMutexObj;

TCHAR string[100];
HANDLE hEvent;
SynchEventMutexObj gSyncObj;

int _tmain(int argvc, TCHAR* argv[]) {
	HANDLE hThreads[2];
	DWORD dwThreadIDs[2];
	gSyncObj.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);		// manual, non-signaled default
	gSyncObj.hMutex = CreateMutex(NULL, FALSE, NULL);			// unoccupied mutex as a default

	if (gSyncObj.hEvent == NULL || gSyncObj.hMutex == NULL) {
		_tprintf(_T("Kernel object creation error, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, outputThreadFunction, NULL, 0, (unsigned *)&dwThreadIDs[0]);
	hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, countThreadFunction, NULL, 0, (unsigned *)&dwThreadIDs[1]);
	if (hThreads[0] == NULL || hThreads[1] == NULL) {
		_tprintf(_T("Thread creation error, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	_fputts(_T("Insert string> "), stdout);
	_fgetts(gSyncObj.string, 30, stdin);
	SetEvent(gSyncObj.hEvent);
	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CloseHandle(gSyncObj.hEvent);
	CloseHandle(gSyncObj.hMutex);
	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	return 0;
}

UINT WINAPI outputThreadFunction(LPVOID lpParam) {
	// Wait until hEvent got signaled
	WaitForSingleObject(gSyncObj.hEvent, INFINITE);
	WaitForSingleObject(gSyncObj.hMutex, INFINITE);
	_tprintf(_T("output string: %s\n"), gSyncObj.string);
	ReleaseMutex(gSyncObj.hMutex);
	return 0;
}

UINT WINAPI countThreadFunction(LPVOID lpParam) {
	// Wait until hEvent got signaled
	WaitForSingleObject(gSyncObj.hEvent, INFINITE);
	WaitForSingleObject(gSyncObj.hMutex, INFINITE);
	_tprintf(_T("output string length: %u\n"), _tcslen(gSyncObj.string) - 1);
	ReleaseMutex(gSyncObj.hMutex);
	return 0;
}