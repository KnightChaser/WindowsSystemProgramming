// Mail receiver (IPC)
// Mailslot IPC is a half-duplex(unidirectional) process communication method.

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

#define  SLOT_NAME			_T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[]) {

	const UINT messageBoxLength = 50;
	HANDLE hMailSlot;							// mailslot handle
	TCHAR* messageBox = malloc(sizeof(TCHAR) * messageBoxLength);
	DWORD bytesRead;

	hMailSlot = CreateMailslot(
		SLOT_NAME,
		0,			// maximum size
		MAILSLOT_WAIT_FOREVER,
		NULL
		);

	// Check mailslot creation success
	if (hMailSlot == INVALID_HANDLE_VALUE) {
		_fputts(_T("Unable to create the mailslot!\n"), stdout);
		return -1;
	}

	// Receiving message from the mailslot
	_fputts(_T("******** MESSAGE ********\n"), stdout);
	while (true) {
		if (!ReadFile(hMailSlot, messageBox, sizeof(TCHAR) * messageBoxLength, &bytesRead, NULL)) {
			_fputts(_T("Unable to read datastream from the mailslot!\n"), stdout);
			CloseHandle(hMailSlot);
			return -1;
		}

		// exit code
		if (!(_tcsncmp(messageBox, _T("exit"), 4))) {
			_fputts(_T("Good bye! UwU\n"), stdout);
			break;
		}

		messageBox[bytesRead / sizeof(TCHAR)] = 0;	// Appending NULL
		_fputts(messageBox, stdout);
	}

	CloseHandle(hMailSlot);
	return 0;

}