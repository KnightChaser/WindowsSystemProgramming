// Manual reset timer object (periodic alert)
#define	_WIN32_WINNT	0x0400
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hTimer = NULL;
	LARGE_INTEGER  timerEventDueTime;
	timerEventDueTime.QuadPart = -100000000;	// relative time(e.g. X time unit since now) is expressed as negative value

	// Create a timer object
	hTimer = CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));
	if (!hTimer) {
		_tprintf(_T("CreateWaitableTimer() got failed. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	_fputts(_T("Set SetWaitableTimer() for 10 seconds.\n"), stdout);
	SetWaitableTimer(hTimer, &timerEventDueTime, 5000, NULL, NULL, FALSE);	// 10 seconds later, the alarm will be sent every 5 seconds

	// Wait for timer alarm
	while (TRUE) {
		WaitForSingleObject(hTimer, INFINITE);
		_fputts(_T("The periodic timer was signaled!\n"), stdout);
		MessageBeep(MB_ICONEXCLAMATION);
	}

	return 0;
}