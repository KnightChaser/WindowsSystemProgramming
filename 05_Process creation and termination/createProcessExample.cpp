#include <iostream>
#include <string>
#include <windows.h>

int main() {
    STARTUPINFO startupInformation;
    ZeroMemory(&startupInformation, sizeof(startupInformation));
    startupInformation.cb = sizeof(startupInformation);
    PROCESS_INFORMATION processInformation;
    ZeroMemory(&processInformation, sizeof(processInformation));
    
    std::wstring command = L"calc.exe";
    SetCurrentDirectory(L"C:\\WINDOWS\\system32");

    if (CreateProcess(NULL,
        const_cast<LPWSTR>(command.c_str()),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startupInformation,
        &processInformation))
    {
        // Wait for the process to finish (optional)
        WaitForSingleObject(processInformation.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(processInformation.hProcess);
        CloseHandle(processInformation.hThread);
    }
    else
    {
        // Handle the error, print an error message
        std::wcerr << L"Error creating process (Error " << GetLastError() << L") for " << command << std::endl;
    }

    return 0;
}
