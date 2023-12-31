// standard output redirection example
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {

	STARTUPINFO startupInformation = { 0, };
	startupInformation.cb = sizeof(startupInformation);
	PROCESS_INFORMATION processInformation;
	SECURITY_ATTRIBUTES fileSecurityAttribute = { 0, };
	fileSecurityAttribute.bInheritHandle = TRUE;		// This should be true for redirection

	TCHAR fileName[] = _T("output.txt");				// The output will be redirected
	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_READ,
		&fileSecurityAttribute, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// The output of the process will be redirected to the file(output.txt <- TCHAR fileName[])
	startupInformation.hStdOutput = hFile;								// set standard output as hFile (file handle)
	startupInformation.hStdInput = GetStdHandle(STD_INPUT_HANDLE);		// set standard input as STD_INPUT_HANDLE(std. input)
	startupInformation.hStdError = GetStdHandle(STD_ERROR_HANDLE);		// set standard output as STD_OUTPUT_HANDLE(std. output)
	startupInformation.dwFlags |= STARTF_USESTDHANDLES;					// logical OR append STARTF_USESTDHANDLES for std I/O handle designation

	TCHAR command[] = _T("example.exe");
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