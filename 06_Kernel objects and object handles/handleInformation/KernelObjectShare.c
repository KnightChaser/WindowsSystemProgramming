// Kernel object share demonstration
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	// The parent process(KernelObjectShare.exe) is now executing its child process, "Operation2.exe"
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

	DWORD timing = 0;
	while (true) {
		for(DWORD i = 0; i < 10000; i++)
			for(DWORD j = 0; j < 10000; j++)
				// Busy waiting

		_fputts(_T("Parent process\n"), stdout);

		// According to Operation2.exe, it was supposed to elevate its priority to
		// HIGH_PRIORITY_CLASS. However, the parent process is now trying to normalize the privilege.
		timing += 1;
		if (timing == 2)
			SetPriorityClass(processInformation.hProcess, NORMAL_PRIORITY_CLASS);
	}
}

// struct of process information { process handle, process thread, process ID, thread ID }
// typedef struct _PROCESS_INFORMATION {
// 	HANDLE hProcess;
//	HANDLE hThread;
//	DWORD dwProcessId;
//	DWORD dwThreadId;
// } PROCESS_INFORMATION, * PPROCESS_INFORMATION, * LPPROCESS_INFORMATION;