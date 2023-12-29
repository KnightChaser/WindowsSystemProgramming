// Mail sender (IPC) 2_2
// Mailslot IPC is a half-duplex(unidirectional) process communication method.

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {
    const UINT messageBoxLength = 50;
    HANDLE hMailSlot;                          // mailslot handle
    TCHAR* message = malloc(sizeof(TCHAR) * messageBoxLength);
    DWORD bytesWritten;

    // Validate the number of command line parameters
    if (argc != 2) {
        _fputts(_T("Usage: MailSender2_2.exe [mailslot handle]\n"), stdout);
        return -1;
    }

    // Convert the mailslot handle from the command line parameter
    hMailSlot = (HANDLE)_tstoi(argv[1]);

    // Validate the mailslot handle
    if (hMailSlot == INVALID_HANDLE_VALUE) {
        _fputts(_T("Invalid mailslot handle!\n"), stdout);
        return -1;
    }

    _tprintf(_T("Inheritable Handle: %d\n"), hMailSlot);
    // now the parent's inheritable kernel object handle table information obtained.

    while (true) {
        _fputts(_T("Mailslot...> "), stdout);
        _fgetts(message, messageBoxLength, stdin);

        // Write a file(data) to the mailslot datastream
        if (!WriteFile(hMailSlot, message, (_tcslen(message) + 1) * sizeof(TCHAR), &bytesWritten, NULL)) {
            _tprintf(_T("Unable to write... (WINAPI32 Error code: %d)\n"), GetLastError());
            _gettchar();
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
