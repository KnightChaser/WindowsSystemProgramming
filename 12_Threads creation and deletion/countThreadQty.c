// CountThread.c
// Measure the number of creatable threads
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define		MAX_THREADS_QTY		1024 * 10

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
	DWORD threadNumber = (DWORD)lpParam;
	while (TRUE) {
		_tprintf(_T("Thread called #%d\n"), threadNumber);
		Sleep(5000);
	}
	return 0;
}

DWORD threadCountQty = 0;

int _tmain(int argc, TCHAR* argv[]) {
	DWORD	dwThreadID[MAX_THREADS_QTY];
	HANDLE	hThread[MAX_THREADS_QTY];

	while (TRUE) {

		// Create the maximum possible threads
		hThread[threadCountQty] =
			CreateThread(
				NULL,							// Default security attribute
				0,								// Default stack size
				ThreadFunction,					// Thread function (function pointer (void*) ptr type)
				(LPVOID)threadCountQty,			// Thread function argument(argv)
				0,								// Default creation flag
				&dwThreadID[threadCountQty]		// Thread ID (may set as NULL)
			);

		// Check thread creation
		if (hThread[threadCountQty] == NULL) {
			_tprintf(_T("Maximum thread count: %d thread(s)\n"), threadCountQty);
			break;
		}

		_tprintf(_T("Current thread: %d thread(s) now\n"), threadCountQty);
		threadCountQty++;
	}

	// Close thread
	for (DWORD i = 0; i < threadCountQty; i++)
		CloseHandle(hThread[i]);

	return 0;
}