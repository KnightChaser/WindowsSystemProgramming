// understanding producer-consumer model
//  			 and synchronized events
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

UINT WINAPI outputThreadFunction(LPVOID lpParam);
UINT WINAPI countThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain(int argvc, TCHAR* argv[]) {
	HANDLE hThread[2];
	DWORD dwThreadID[2];

	hEvent = CreateEvent(
		NULL,		// Unable to inherit
		TRUE,		// manual-reset mode
		FALSE,		// non-signaled status => set non-signaled(thread got blocked) as default. Manually signal to unblock
		NULL		// unnamed event
	);

	if (hEvent == NULL) {
		_tprintf(_T("Failed to create an event object, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, outputThreadFunction, NULL, 0, (unsigned *)&dwThreadID[0]);	// Thread[0] is about to run (but blocked by hEvent)
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, countThreadFunction, NULL, 0, (unsigned *)&dwThreadID[1]);		// Thread[1] is about to run (but blocked by hEvent)
	if (hThread[0] == NULL || hThread[1] == NULL) {
		_tprintf(_T("Failed to create threads, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	_fputts(_T("Insert string> "), stdout);
	_fgetts(string, 30, stdin);
	SetEvent(hEvent);		// event's state is now changed to signaled => Wake up multiple sleeping processes
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(hEvent);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	return 0;
}

UINT WINAPI outputThreadFunction(LPVOID lpParam) {
	// Wait until hEvent got signaled
	WaitForSingleObject(hEvent, INFINITE);
	_tprintf(_T("output string: %s\n"), string);
	return 0;
}

UINT WINAPI countThreadFunction(LPVOID lpParam) {
	// Wait until hEvent got signaled
	WaitForSingleObject(hEvent, INFINITE);
	_tprintf(_T("output string length: %d\n"), _tcslen(string) - 1);
	return 0;
}