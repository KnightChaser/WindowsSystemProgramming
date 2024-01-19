// Custom exception raise
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void SoftwareException(void);

int _tmain(int argc, TCHAR* argv[]) {
	SoftwareException();
	_fputts(_T("End of _tmain()\n"), stdout);
	
	return 0;
}

// dwExceptionCode
void SoftwareException(void) {
	DWORD DefinedException = 0x00;
	
	// Severity
	DefinedException |= 0x01 << 31;
	DefinedException |= 0x01 << 30;

	// MS/C (Microsoft/Custom-defined bits)
	DefinedException |= 0x01 << 29;

	// Reserved bit (must be 0)
	DefinedException |= 0x00 << 28;

	// Facility code
	DefinedException |= 0x00 << 16;

	// Exception code (developers arbitrary assign this code)
	DefinedException |= 0x08;		// FACILITY_WINDOWS

	__try {
		_tprintf(_T("Send: Exception code: 0x%x\n"), DefinedException);
		RaiseException(DefinedException, 0, NULL, NULL);		// trigger the custom-defined exception
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		DWORD exceptionType = GetExceptionCode();
		_tprintf(_T("Recv: Exception code: 0x%x\n"), exceptionType);
		
		// By inspecting MS/C field of dwExceptionCode, we can know that if the current exception is defined by developers of MSFT
		if (exceptionType & (0x01 << 29))
			_fputts(_T("Developers defined exception!\n"), stdout);
		else
			_fputts(_T("Official Microsoft defined exception\n"), stdout);
	}

	return 0;

}