// Custom advanced version of "AdderManager.c"
// You can dynamically adjust the number of creating child processes.
// - You need to just look at childProcessCreationQty and commands
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define childProcessCreationQty 5

int _tmain(int argc, TCHAR* argv[]) {

    // Set up for windows processes
    STARTUPINFO startupInformationArray[childProcessCreationQty];
    memset(startupInformationArray, 0, sizeof(startupInformationArray));
    PROCESS_INFORMATION processInformationArray[childProcessCreationQty];
    DWORD returnValuesFromChildren[childProcessCreationQty];
    TCHAR* commands[childProcessCreationQty] = {
        _T("PartAdder.exe 1 5"),
        _T("PartAdder.exe 6 10"),
        _T("PartAdder.exe 11 15"),
        _T("PartAdder.exe 16 20"),
        _T("PartAdder.exe 21 25")
    };
    DWORD sum = 0;

    // Sequentially setup process structure and create processes
    for (DWORD index = 0; index < childProcessCreationQty; index++) {
        startupInformationArray[index].cb = sizeof(startupInformationArray[index]);
        TCHAR commandLine[MAX_PATH]; // Assuming MAX_PATH is sufficient for your command line
        _tcscpy_s(commandLine, MAX_PATH, commands[index]);

        BOOL createProcessStatus = CreateProcess(
            NULL,
            commandLine,
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            NULL,
            &startupInformationArray[index],
            &processInformationArray[index]
        );

        if (createProcessStatus == 0) {
            // Handle process creation failure
            _tprintf(_T("Failed to create %uth process.\n"), index + 1);
            return -1;
        } else 
            _tprintf(_T("Successfully created %uth process.\n"), index + 1);
    }

    for (DWORD index = 0; index < childProcessCreationQty; index++) {
        // Prevent process information leakage
        CloseHandle(&processInformationArray[index].hThread);

        // Wait until the child process finishes its operation, and receive the return value from the children
        WaitForSingleObject(processInformationArray[index].hProcess, INFINITE);
        GetExitCodeProcess(processInformationArray[index].hProcess, &returnValuesFromChildren[index]);
    }

    // Check abnormal shutdown
    for (DWORD index = 0; index < childProcessCreationQty; index++) {
        if (returnValuesFromChildren[index] == -1) {
            _tprintf(_T("%dth process exited abnormally\n"), index + 1);
            return -1;
        }
        else {
            sum += returnValuesFromChildren[index];
        }
    }

    _tprintf(_T("Total: %d\n"), sum);

    // Close handles of children to exit
    for (DWORD index = 0; index < childProcessCreationQty; index++)
        CloseHandle(&processInformationArray[index].hProcess);

    return 0;
}
