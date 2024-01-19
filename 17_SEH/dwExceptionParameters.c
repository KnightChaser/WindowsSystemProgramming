// Observing RaiseException parameters
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define STATUS_DEFAULT_USER_DEFINED_EXCEPTION ((DWORD)0xE0000008L)

void SoftwareException(void);
DWORD FilterFunction(LPEXCEPTION_POINTERS lpExceptionPtr);

int _tmain(int argc, _TCHAR* argv[]) {
	SoftwareException();
	_fputts(_T("End of _tmain()\n"), stdout);

	return 0;
}

void SoftwareException(void) {
	TCHAR* lpExceptionString[] = {
		_T("Exception string #1"),
		_T("Exception string #2"),
		_T("Exception string #3")
	};

	__try {
		RaiseException(STATUS_DEFAULT_USER_DEFINED_EXCEPTION,
			0, 3, (ULONG_PTR*)lpExceptionString);
	} 
	__except (FilterFunction(GetExceptionInformation())) {
		DWORD exceptionType = GetExceptionCode();
		_tprintf(_T("Received an exception code: 0x%x\n"), exceptionType);
	}
}

DWORD FilterFunction(LPEXCEPTION_POINTERS lpExceptionPtr) {
	PEXCEPTION_RECORD lpExceptionRecord = lpExceptionPtr->ExceptionRecord;
	PCONTEXT lpContextRecord = lpExceptionPtr->ContextRecord;

	switch (lpExceptionRecord->ExceptionCode) {
	case STATUS_DEFAULT_USER_DEFINED_EXCEPTION:
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] Exception code: 0x%x\n"), lpExceptionRecord->ExceptionCode);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] Exception flags: 0x%x\n"), lpExceptionRecord->ExceptionFlags);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] Exception # of parameters: 0x%x\n"), lpExceptionRecord->NumberParameters);

		for (DWORD index = 0; index < lpExceptionRecord->NumberParameters; index++)
			_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] Exception string (Index #%d): %s\n"),
				index, (TCHAR*)lpExceptionRecord->ExceptionInformation[index]);

		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] RAX: 0x%I64x\n"), lpContextRecord->Rax);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] RBX: 0x%I64x\n"), lpContextRecord->Rbx);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] RIP: 0x%I64x\n"), lpContextRecord->Rip);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] RSI: 0x%I64x\n"), lpContextRecord->Rsi);
		_tprintf(_T("[STATUS_DEFAULT_USER_DEFINED_EXCEPTION] RSP: 0x%I64x\n"), lpContextRecord->Rsp);

		break;

	default:
		break;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
