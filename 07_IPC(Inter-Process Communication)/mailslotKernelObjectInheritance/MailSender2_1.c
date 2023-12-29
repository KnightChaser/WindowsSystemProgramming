// Mail sender (IPC) 2_1
// Mailslot IPC is a half-duplex(unidirectional) process communication method.

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[]) {
    const UINT messageBoxLength = 50;
    HANDLE hMailSlot;                          // mailslot handle
    TCHAR* message = malloc(sizeof(TCHAR) * messageBoxLength);
    DWORD bytesWritten;

    // Manually create and adjust SECURITY_ATTRIBUTES to inherit kernel object handle table
    SECURITY_ATTRIBUTES securityAttributes;
    securityAttributes.nLength = sizeof(securityAttributes);
    securityAttributes.lpSecurityDescriptor = NULL;
    securityAttributes.bInheritHandle = TRUE;

    // Create a file writing data stream
    hMailSlot = CreateFile(
        SLOT_NAME,                 // the name of mailslot
        GENERIC_WRITE,             // usage(Read? Write?)
        FILE_SHARE_READ,
        &securityAttributes,       // SECURITY_ATTRIBUTE (make it able to inherit kernel object handle table)
        OPEN_EXISTING,             // creation method
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    // Check mailslot creation success
    if (hMailSlot == INVALID_HANDLE_VALUE) {
        _fputts(_T("Unable to create the mailslot!\n"), stdout);
        return -1;
    }

    STARTUPINFO startupInformation = { 0, };
    startupInformation.cb = sizeof(startupInformation);
    PROCESS_INFORMATION processInformation;
    TCHAR command[256];    // Assuming a reasonable buffer size for the command

    // Build the command with the mailslot handle as a parameter
    _stprintf_s(command, _countof(command), _T("MailSender2_2.exe %d"), hMailSlot);

    // The child process got inherited the parent process information
    // Thus, it can send data to the mailslot "\\.\mailslot\mailbox".
    CreateProcess(
        NULL,
        command,
        NULL,
        NULL,
        TRUE,        // set TRUE of kernel object handle table inheritance
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &startupInformation,
        &processInformation
    );

    while (true) {
        _fputts(_T("Mailslot...> "), stdout);
        _fgetts(message, messageBoxLength, stdin);

        // Write a file(data) to the mailslot datastream
        if (!WriteFile(hMailSlot, message, (_tcslen(message) + 1) * sizeof(TCHAR), &bytesWritten, NULL)) {
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
