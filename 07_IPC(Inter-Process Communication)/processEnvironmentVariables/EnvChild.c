// EnvChild.c
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUFFER_SIZE 1024

int _tmain(int argc, TCHAR* argv[]) {

	_fputts(_T("Child process(EnvChild.c)\n"), stdout);

	// A buffer that the enviorn from the parent process will be saved into
	TCHAR value[BUFFER_SIZE];
	TCHAR* targetEnvironKey = _T("Hello");

	if (GetEnvironmentVariable(targetEnvironKey, value, BUFFER_SIZE) > 0)
		_tprintf(_T("environ: %s => %s\n"), targetEnvironKey, value);
	else if(GetLastError() == ERROR_ENVVAR_NOT_FOUND)
		_tprintf(_T("Could not load variable \"%s\".\n"), targetEnvironKey);

	_gettchar();
	return 0;

}