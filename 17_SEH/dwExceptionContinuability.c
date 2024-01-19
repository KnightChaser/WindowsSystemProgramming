// RaiseException dwExceptionCode.dwExceptionFlag
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BEST_TEST_EXCEPTION ((DWORD)0xE0000008L)

void SoftwareException(void);

int _tmain(int argc, _TCHAR* argv[]) {
	SoftwareException();
	_fputts(_T("End of _tmain()\n"), stdout);

	return 0;
}

void SoftwareException(void) {
	__try {
		RaiseException(BEST_TEST_EXCEPTION, 0, NULL, NULL);
		_fputts(_T("Well, this code is also working.\n"), stdout);
	}
	// __except (EXCEPTION_CONTINUE_EXECUTION) {		// handle the exception and continue procedure
	__except (EXCEPTION_NONCONTINUABLE) {				// handle the exception and abort procedure
		DWORD exceptionType = GetExceptionCode();
		if (exceptionType == BEST_TEST_EXCEPTION) {
			_fputts(_T("An exception BEST_TEST_EXCEPTION got triggered.\n"), stdout);
		}
	}
}