// Getting predefined exception information
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	int* p = NULL;
	int selection = 0;

	while (TRUE) {
		_fputts(_T("1 for memmory access exception.\n"), stdout);
		_fputts(_T("2 for divide by 0 exception.\n"), stdout);
		_fputts(_T("Your choice(3 for exiting...) > "), stdout);
		_tscanf(_T("%d"), &selection);

		if (selection == 3)
			break;

		__try {
			if (selection == 1) {
				*p = 100;			// Will trigger __except => EXCEPTION_ACCESS_VIOLATION
				_tprintf(_T("value: %d\n"), *p);
			}
			else {
				INT n = 0;			// Will trigger __except => EXCEPTION_INT_DIVIDE_BY_ZERO
				n = 7 / n;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			DWORD exceptionType = GetExceptionCode();
			switch (exceptionType) {
			case EXCEPTION_ACCESS_VIOLATION:
				_fputts(_T("EXCEPTION_ACCESS_VIOLATION\n"), stdout);
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				_fputts(_T("EXCEPTION_INT_DIVIDE_BY_ZERO\n"), stdout);
				break;
			}
		}
	}

	return 0;

}