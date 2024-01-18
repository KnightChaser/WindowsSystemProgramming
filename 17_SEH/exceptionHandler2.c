#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	INT x, y, result;
	_fputts(_T("x y > "), stdout);
	_tscanf(_T("%d %d"), &x, &y);

	__try {
		__try {
			// You should wanna put 0 in y, right?
			result = divide(x, y);
			_tprintf(_T("%d / %d => %d\n"), x, y, result);
		}
		__finally {
			// Even though the exception occurs, this block will be executed anyway.
			_fputts(_T("End of operation\n"), stdout);
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		_fputts(_T("It seems that wrong numbers inserted.\n"), stdout);
	}

	return 0;
}

INT divide(int x, int y) {
	return x / y;
}