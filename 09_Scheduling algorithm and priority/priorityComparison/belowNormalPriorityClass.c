// belowNormalPriorityClass.c
// Just printing something
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

	while (TRUE) {
		for (DWORD i = 0; i < 10000; i++)
			for (DWORD j = 0; j < 10000; j++);
				// Busy waiting
		// Sleep(10);
		_fputts(_T("BELOW_NORMAL_PRIORITY_CLASS process\n"), stdout);
	}

	return 0;
}