// EnvParent.c
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	// Register environmental registers
	SetEnvironmentVariable(_T("Hello"), _T("World"));

	// Create a child process
	STARTUPINFO startupInformation = { 0, };
	PROCESS_INFORMATION processInformation = { 0, };
	startupInformation.cb = sizeof(startupInformation);
	TCHAR childProcessName[] = _T("EnvChild.exe");

	if (!CreateProcess(
		NULL, childProcessName, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE,
		NULL, NULL, &startupInformation, &processInformation)) {
		_tprintf(_T("Failed to create a child process. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}
	else
		_fputts(_T("Successfully created a child process. Let's check it out!"), stdout);

	// Wait until the child process finishes working
	WaitForSingleObject(processInformation.hProcess, INFINITE);
	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);

	return 0;
}