// DuplicateHandle.c

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	HANDLE hProcess;
	TCHAR cmdString[1024];

	// By using DuplicateHandle(), transform the pseudo process handle obtained from GetCurrentProcess()
	// into the real process handle of the current(parent) process handlem and save it to &hProcess(HANDLE) variable.
	DuplicateHandle(
		GetCurrentProcess(),
		GetCurrentProcess(),
		GetCurrentProcess(),
		&hProcess,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS
	);

	_stprintf_s(cmdString, _countof(cmdString), _T("%s %u"), _T("DuplicateHandleChildProcess.exe"), (unsigned)hProcess);

	STARTUPINFO startupInformation = { 0, };
	startupInformation.cb = sizeof(startupInformation);
	PROCESS_INFORMATION processInformation = { 0, };

	BOOL isCreateProcessSuccessful = CreateProcess(
		NULL,
		cmdString,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startupInformation,
		&processInformation
	);

	if (isCreateProcessSuccessful == FALSE) {
		_fputts(_T("The creation task of a child process was unsuccessful.\n"), stdout);
		return -1;
	}

	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);

	_fputts(_T("[Parent Process]\n"), stdout);
	_tprintf(_T("The current(parent) process handle value: %d\n"), hProcess);
	_fputts(_T("OwO\n"), stdout);

	return 0;

}