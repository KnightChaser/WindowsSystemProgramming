// Retrieve the process handle
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	// Set current directory's process priority to HIGH_PRIORITY_CLASS
	// now run Operation1.exe. In single core and relatively slow CPU,
	// Operation2.exe will almost take CPU time over Operation1.exe
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	while(true) {
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD j = 0; j < 10000; j++);
				// just doing some busy waiting

		_fputts(_T("Operation2.exe \n"), stdout);
	}

	return 0;
}