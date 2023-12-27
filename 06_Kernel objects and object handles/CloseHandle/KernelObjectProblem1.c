// KernelObjectProblem1.c
// Understanding CloseHandle()

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	// The parent process(KernelObjectShare.exe) is now executing its child process, "Operation2.exe"
	STARTUPINFO startupInformation = { 0, };
	PROCESS_INFORMATION processInformation;
	startupInformation.cb = sizeof(startupInformation);
	TCHAR command[] = _T("KernelObjectProblem2.exe");

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

	CloseHandle(processInformation.hProcess);

	return 0;
}