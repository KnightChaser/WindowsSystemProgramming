// listProcessInformation.c
// -> Print the information of currently working processes

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>

int _tmain(int argc, TCHAR* argv[]) {

	// Take a snapshot of the current every working process(TH32CS_SNAPPROCESS)
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateToolhelp32Snapshot() got an error! Error code: %d\n"), GetLastError());
		return -1;
	}

	// A struct object to obtain process information
	// Process32First() retrieves information about the first process encountered in a system snapshot.
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnapshot, &processEntry32)) {
		_tprintf(_T("Process32First() got an error! Error code: %d\n"), GetLastError());
		CloseHandle(hProcessSnapshot);
		return -1;
	}

	do {
		// Print process name and inforamtion
		_tprintf(_T("%40s[PID#%6d]\n"), processEntry32.szExeFile, processEntry32.th32ProcessID);
	} while (Process32Next(hProcessSnapshot, &processEntry32));

	CloseHandle(hProcessSnapshot);
	return 0;

}