// KernelObjectProblem2.c
// Understanding CloseHandle()

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdbool.h>

int _tmain(int argc, TCHAR* argv[]) {

	DWORD n = 0;

	while (n < 100) {
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD j = 0; j < 10000; j++);
				// just busy waitin'

		_fputts(_T("KernelObjectProblem2.exe\n"), stdout);
		n++;
	}

	return 0;
}