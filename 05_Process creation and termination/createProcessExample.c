#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	STARTUPINFO startupInformation;
	ZeroMemory(&startupInformation, sizeof(startupInformation));
	startupInformation.cb = sizeof(startupInformation);
	PROCESS_INFORMATION processInformation;
	ZeroMemory(&processInformation, sizeof(processInformation));

	TCHAR command[] = _T("calc.exe");
	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));

	BOOL isProcessSuccessfullyCreated 
        = CreateProcess(NULL,                   // lpApplicationName
                        command,                // lpCommandLine
                        NULL,                   // lpProcessAttributes
                        NULL,                   // lpThreadAttributes
                        TRUE,                   // bInheritHandles
                        0,                      // dwCreationFlags
                        NULL,                   // lpEnvironment
                        NULL,                   // lpCurrentDirectory
                        &startupInformation,    // lpStartupInfo
                        &processInformation);   // lpProcessInformation

	if (!isProcessSuccessfullyCreated) {
		_tprintf(_T("Failed to create a process \"%s\", obtained an error code: %d\n"), command, GetLastError());
		return -1;
	}

	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);
	return 0;
}