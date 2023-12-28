// Mail sender (IPC)
// Mailslot IPC is a half-duplex(unidirectional) process communication method.

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

#define  SLOT_NAME			_T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[]) {

	const UINT messageBoxLength = 50;
	HANDLE hMailSlot;							// mailslot handle
	TCHAR* message = malloc(sizeof(TCHAR) * messageBoxLength);
	DWORD bytesWritten;

	// Create a file writing data stream
	hMailSlot = CreateFile(
		SLOT_NAME,					// the name of mailslot
		GENERIC_WRITE,				// usage(Read? Write?)
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,				// creation method
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// Check mailslot creation success
	if (hMailSlot == INVALID_HANDLE_VALUE) {
		_fputts(_T("Unable to create the mailslot!\n"), stdout);
		return -1;
	}

	while (true) {
		_fputts(_T("Mailslot...> "), stdout);
		_fgetts(message, messageBoxLength, stdin);

		// Write a file(data) to the mailslot datastream
		if (!WriteFile(hMailSlot, message, _tcslen(message) * sizeof(TCHAR), &bytesWritten, NULL)) {
			_fputts(_T("Unable to write...\n"), stdout);
			CloseHandle(hMailSlot);
			return -1;
		}

		// exit code
		if (!(_tcsncmp(message, _T("exit"), 4))) {
			_fputts(_T("Good bye! UwU\n"), stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);
	return 0;

}