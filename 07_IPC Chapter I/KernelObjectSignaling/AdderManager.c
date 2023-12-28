// PartAdder
// This function will call multiple child processes(PartAdder.exe) that receives two integers x, y as execution arguments
// and returns the sum x to y. This PartAdder.exe captures such returning codes from the child processes by using
// GetExitCodeProcess(), and manage the values.

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	STARTUPINFO startupInformation1 = { 0, };
	STARTUPINFO startupInformation2 = { 0, };
	PROCESS_INFORMATION processInformation1;
	PROCESS_INFORMATION processInformation2;
	DWORD returnValue1;
	DWORD returnValue2;
	TCHAR command1[] = _T("PartAdder.exe 1 5");
	TCHAR command2[] = _T("PartAdder.exe 6 10");
	DWORD sum = 0;

	startupInformation1.cb = sizeof(startupInformation1);
	startupInformation2.cb = sizeof(startupInformation2);

	CreateProcess(
		NULL,
		command1,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&startupInformation1,
		&processInformation1
	);	// CreateProcess() for the first process (PartAdder.exe 1 5)

	CreateProcess(
		NULL,
		command2,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&startupInformation2,
		&processInformation2
	);	// CreateProcess() for the second process (PartAdder.exe 6 10)

	CloseHandle(processInformation1.hThread);
	CloseHandle(processInformation2.hThread);

	// Wait until the child process finish its operation
	WaitForSingleObject(processInformation1.hProcess, INFINITE);
	WaitForSingleObject(processInformation2.hProcess, INFINITE);

	// Get the return values from each child process
	GetExitCodeProcess(processInformation1.hProcess, &returnValue1);
	GetExitCodeProcess(processInformation2.hProcess, &returnValue2);

	// Abnormal shutdown
	if (returnValue1 == -1 || returnValue2 == -1)
		return -1;

	sum += returnValue1;
	sum += returnValue2;

	_tprintf(_T("Total: %d\n"), sum);

	CloseHandle(processInformation1.hProcess);
	CloseHandle(processInformation2.hProcess);

	return 0;

}