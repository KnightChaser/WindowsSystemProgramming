#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	_fputts(_T("start point\n"), stdout);
	int* p = NULL;		// Creating an invalid pointer

	__try {
		*p = 100;		// exception occcurs here
		_tprintf(_T("p value: %d\n"), p);	// no executed because the upper line is exception-occuring
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		_fputts(_T("exception occured!\n"), stdout);
	}

	_fputts(_T("end point\n"), stdout);
	return 0;

}