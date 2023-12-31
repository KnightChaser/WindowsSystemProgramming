// standard output redirection example II (sort.exe)
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#define MAX_FILENAME_LENGTH 1024

int _tmain(int argc, TCHAR* argv[]) {

	TCHAR outputRedirectedFilename[MAX_FILENAME_LENGTH] = { 0, };
	_fputts(_T("output file name for result of \"sort.exe\"> "), stdout);
	if (!(_tscanf_s(_T("%s"), outputRedirectedFilename, (UINT)_countof(outputRedirectedFilename))) > 0) {
		_tprintf(_T("The file name hasn't been appropriately designated. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	STARTUPINFO startupInformation = { 0, };
	startupInformation.cb = sizeof(startupInformation);
	PROCESS_INFORMATION processInformation;
	SECURITY_ATTRIBUTES fileSecurityAttribute = { 0, };
	fileSecurityAttribute.bInheritHandle = TRUE;		// This should be true for redirection

	HANDLE hFile = CreateFile(
		outputRedirectedFilename, GENERIC_WRITE, FILE_SHARE_READ,
		&fileSecurityAttribute, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// The output of the process will be redirected to the file(output.txt <- TCHAR fileName[])
	startupInformation.hStdOutput = hFile;								// set standard output as hFile (file handle)
	startupInformation.hStdInput = GetStdHandle(STD_INPUT_HANDLE);		// set standard input as STD_INPUT_HANDLE(std. input)
	startupInformation.hStdError = GetStdHandle(STD_ERROR_HANDLE);		// set standard output as STD_OUTPUT_HANDLE(std. output)
	startupInformation.dwFlags |= STARTF_USESTDHANDLES;					// logical OR append STARTF_USESTDHANDLES for std I/O handle designation

	SetCurrentDirectory(_T("C:\\WINDOWS\\system32"));
	TCHAR command[] = _T("sort.exe");
	_tprintf(_T("\"sort.exe\" started and the output will be redirected to \"%s\". Type ^z to exit.\n"), outputRedirectedFilename);
	if (!CreateProcess(NULL, command, NULL, NULL,
		TRUE, 0, NULL, NULL, &startupInformation, &processInformation)) {
		_tprintf(_T("Failed to create a chlid process. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	WaitForSingleObject(processInformation.hProcess, INFINITE);
	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);

	return 0;

}