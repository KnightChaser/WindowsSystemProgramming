// Anonymous Pipe
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	
	HANDLE hReadPipe;	// Anonymous pipe handle for reading
	HANDLE hWritePipe;	// Anonymous pipe handle for writing
	TCHAR recvString[100];
	TCHAR sendString[] = _T("Anonymous Pipe OwO");
	DWORD bytesRead;
	DWORD bytesWritten;

	// Create an anonymous pipe
	CreatePipe(&hReadPipe, &hWritePipe, NULL, 0);

	// Send datastream through the anonymous pipe(send)
	WriteFile(
		hWritePipe, sendString,
		sizeof(sendString),
		&bytesWritten,
		NULL);
	_tprintf(_T("string send to the pipe: %s\n"), sendString);

	// Receive datastream through the anonymous pipe(receive)
	BOOL isPipeDataTransmissionSuccessful = ReadFile(
		hReadPipe, recvString,
		bytesWritten,
		&bytesRead,
		NULL);

	if (isPipeDataTransmissionSuccessful != TRUE) {
		_fputts(_T("The data transmission through the anonymous pipe was unsuccessful"), stdout);
		return -1;
	}
	recvString[bytesRead / sizeof(TCHAR)] = NULL;	// NULL termination
	_tprintf(_T("string received from the pipe: %s\n"), recvString);

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);

	return 0;
}