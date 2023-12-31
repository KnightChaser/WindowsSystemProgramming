// Receive a process name(e.g. CalculatorApp.exe for Windows Calculator(WIN11)) and kill it
// Process name --(processEntry32)--> ProcessID --(OpenProcess)--> ProcessHANDLE --(ProcessTerminate)--> *process taskkill*
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>

BOOL terminateProcess(TCHAR* targetProcessName);

int _tmain(int argc, TCHAR* argv[]) {

	// Receive the target process information from the user
	_fputts(_T("Custom TASKKILL OwO\n"), stdout);
	TCHAR targetProcessName[1024] = { 0, };
	_fputts(_T("Enter the target process name to kill> "), stdout);
	_tscanf(_T("%s"), targetProcessName);

	terminateProcess(targetProcessName);

	return 0;

}

/// <summary>
///  Custom taskkill(Doing the same thing with taskkill.exe in Windows)
/// </summary>
/// <param name="targetProcessName">Enter the target process name like CalculatorApp.exe or Notepad.exe</param>
/// <returns>FALSE if the task fails, TRUE if the task succeeds</returns>
BOOL terminateProcess(TCHAR* targetProcessName) {
	// Take a snapshot of the current every working process(TH32CS_SNAPPROCESS)
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot() got an error! Error code: %d\n"), GetLastError());
		return FALSE;
	}

	// A struct object to obtain process information
	// Process32First() retrieves information about the first process encountered in a system snapshot.
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &processEntry32)) {
		_tprintf(_T("Process32First() got an error! Error code: %d\n"), GetLastError());
		CloseHandle(hProcessSnapshot);
		return FALSE;
	}

	// Search the target process's ID(PID)
	BOOL isTargetProcessFound = FALSE;
	INT targetProcessID = -1;
	do {
		if (!(_tcscmp(processEntry32.szExeFile, targetProcessName))) {
			isTargetProcessFound = TRUE;
			targetProcessID = processEntry32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnapshot, &processEntry32));

	if (!isTargetProcessFound) {
		_tprintf(_T("Could not find the target process(%s).\n"), targetProcessName);
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) PID found => %d\n"), targetProcessName, targetProcessID);

	// Search the target process handle(HANDLE) based on the process ID
	HANDLE targetProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessID);
	if (targetProcessHandle == NULL) {
		_tprintf(_T("Failed to obtain the target process(%s) Handle\n"), targetProcessName);
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) handle found => %llu\n"), targetProcessName, (ULONG_PTR)targetProcessHandle);

	// Terminate the target process by the obtained HANDLE value
	BOOL isProcessTerminated = TerminateProcess(targetProcessHandle, 0);
	if (!isProcessTerminated) {
		_tprintf(_T("Failed to terminate the process(%s(PID: %d)) with handle(%llu). Got an error code %d\n"),
			targetProcessName, targetProcessID, (ULONG_PTR)targetProcessHandle, GetLastError());
		return FALSE;
	}
	else
		_tprintf(_T("Target process(%s) successfully termianted.\n"), targetProcessName);

	// finished(successfully)
	return TRUE;

}