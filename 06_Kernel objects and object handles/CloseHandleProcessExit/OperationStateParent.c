// OperationStateParent.c
// Check return value according to the program execution result

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	STARTUPINFO startupInformation = { 0, };
	PROCESS_INFORMATION processInformation;
	DWORD state;

	startupInformation.cb = sizeof(startupInformation);
	startupInformation.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startupInformation.dwX = 100;
	startupInformation.dwY = 200;
	startupInformation.dwXSize = 300;
	startupInformation.dwYSize = 200;
	startupInformation.lpTitle = _T("return 'n exit");

	TCHAR command[] = _T("OperationStateChild.exe");

	CreateProcess(
		NULL,
		command,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&startupInformation,
		&processInformation
	);

	for (DWORD i = 0; i < 10000; i++)
		for (DWORD j = 0; j < 10000; j++);
	// just for busy waiting

	GetExitCodeProcess(processInformation.hProcess, &state);
	if (state == STILL_ACTIVE)
		_tprintf(_T("STILL_ACTIVE\n"));
	else
		_tprintf(_T("state: %d\n"), state);

	// CloseHandle(processInformation.hProcess);

	return 0;
}