// Manual reset timer object (periodic alert)
#define	_WIN32_WINNT	0x0400
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	HANDLE hTimer = NULL;
	LARGE_INTEGER  timerEventDueTime;
	timerEventDueTime.QuadPart = -20000000;	// relative time(e.g. X time unit since now) is expressed as negative value

	// Create a timer object
	hTimer = CreateWaitableTimer(NULL, FALSE, _T("WaitableTimer"));
	if (!hTimer) {
		_tprintf(_T("CreateWaitableTimer() got failed. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	_fputts(_T("Set SetWaitableTimer() for 5 seconds.\n"), stdout);
	SetWaitableTimer(hTimer, &timerEventDueTime, 1000, NULL, NULL, FALSE);	// 2 seconds later, the alarm will be sent every 1 seconds

	// Wait for timer alarm
	DWORD timerTriggerQty = 0;
	while (TRUE) {
		WaitForSingleObject(hTimer, INFINITE);
		timerTriggerQty += 1;
		_tprintf(_T("The periodic timer was signaled for %d times!\n"), timerTriggerQty);
		MessageBeep(MB_ICONEXCLAMATION);
		if (timerTriggerQty >= 10) {
			CancelWaitableTimer(hTimer);
			CloseHandle(hTimer);
			break;
		}
	}

	return 0;
}