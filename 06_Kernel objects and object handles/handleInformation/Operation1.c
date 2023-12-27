// Retrieve the process handle
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	// run "./Operation2.exe"
	STARTUPINFO startupInformation = { 0, };
	PROCESS_INFORMATION processInformation;
	startupInformation.cb = sizeof(startupInformation);
	TCHAR command[] = _T("Operation2.exe");

	CreateProcess(
		NULL,
		command,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&startupInformation,
		&processInformation
	);

	CloseHandle(processInformation.hThread);
	CloseHandle(processInformation.hProcess);

	// After a busy waiting, print "Operation1.exe" strin literal
	while (true) {
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD j = 0; j < 10000; j++);
				// just doing some busy waiting

		_fputts(_T("Operation1.exe \n"), stdout);

	}

	return 0;
}