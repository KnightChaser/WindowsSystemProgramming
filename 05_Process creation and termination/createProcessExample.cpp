#include <iostream>
#include <string>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[]) {
    STARTUPINFO startupInformation;
    ZeroMemory(&startupInformation, sizeof(startupInformation));
    startupInformation.cb = sizeof(startupInformation);
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&processInformation, sizeof(processInformation));

    std::wstring command = L"calc.exe";
    SetCurrentDirectory(L"C:\\WINDOWS\\system32");


    bool isProcessSuccessfullyCreated
        = CreateProcess(NULL,                                   // lpApplicationName
                        const_cast<LPTSTR>(command.c_str()),    // lpCommandLine
                        NULL,                                   // lpProcessAttributes
                        NULL,                                   // lpThreadAttributes
                        TRUE,                                   // bInheritHandles
                        0,                                      // dwCreationFlags
                        NULL,                                   // lpEnvironment
                        NULL,                                   // lpCurrentDirectory
                        &startupInformation,                    // lpStartupInfo
                        &processInformation);                   // lpProcessInformation
    if (!isProcessSuccessfullyCreated) {
        // Handle the error, print an error message
        std::wcerr << L"Error creating process (Error " << GetLastError() << L") for " << command << std::endl;
    }

    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);

    return 0;
}
