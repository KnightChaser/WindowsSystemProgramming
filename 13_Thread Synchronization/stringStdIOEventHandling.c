// understanding producer-consumer model
//  			 and synchronized events
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

UINT WINAPI outputThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hThread;
	DWORD dwThreadID;

	// Create an event object
	hEvent = CreateEvent(
		NULL,	// unable to inherit
		TRUE,	// manual-reset mode (signal/non-signal status change should be done manually)
		FALSE,	// create as a non-signaled status initially
		NULL	// unnamed event
	);

	if (hEvent == NULL) {
		_tprintf(_T("Failed to create an event object, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	hThread = (HANDLE)_beginthreadex(
		NULL, 0, outputThreadFunction,
		NULL, 0, (unsigned*)&dwThreadID);

	if (hThread == 0) {
		_tprintf(_T("Failed to create the thread, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(string, 30, stdin);
	SetEvent(hEvent);		// change "hEvent"'s state to signaled
							// Until this "checkpoint", hEvent will still be remained as blocked

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hEvent);	// event object got destroyed
	CloseHandle(hThread);

	return 0;
}

UINT WINAPI outputThreadFunction(LPVOID lpParam) {
	// Wait untnil the event to be "signaled"
	WaitForSingleObject(hEvent, INFINITE);
	_tprintf(_T("Output string: %s\n"), string);

	return 0;
}