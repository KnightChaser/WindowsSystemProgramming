// aboveNormalPriorityClass.c
// Spawning two processes with different priorities
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	STARTUPINFO startupInformationNormal = { 0, };
	PROCESS_INFORMATION processInformationNormal;
	TCHAR commandNormalProcess[] = _T("normalPriorityClass.exe");

	STARTUPINFO startupInformationBelowNormal = { 0, };
	PROCESS_INFORMATION processInformationBelowNormal;
	TCHAR coommandNormalBelowProcess[] = _T("belowNormalPriorityClass.exe");

	startupInformationNormal.cb = sizeof(startupInformationNormal);
	startupInformationBelowNormal.cb = sizeof(startupInformationBelowNormal);

	// Set the current process priority
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// Create subprocesses (ref. to the priorities)
	CreateProcess(NULL, commandNormalProcess, NULL, NULL, TRUE, 0, NULL, NULL, &startupInformationNormal, &processInformationNormal);
	CreateProcess(NULL, coommandNormalBelowProcess, NULL, NULL, TRUE, 0, NULL, NULL, &startupInformationBelowNormal, &processInformationBelowNormal);

	while (TRUE) {
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD j = 0; j < 10000; j++);
				// Busy waiting
		Sleep(10);
		_fputts(_T("ABOVE_NORMAL_PRIORITY_CLASS process\n"), stdout);
	}

	return 0;
}