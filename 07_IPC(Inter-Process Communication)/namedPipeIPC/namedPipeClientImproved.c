// Named pipe client (IPC)
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUFFER_SIZE 1024

void doNamedPipelineIPCClientTask();

int _tmain(int argc, TCHAR* argv[]) {

	// You can try IPC multiple times.
	while (TRUE) {
		doNamedPipelineIPCClientTask();
	}

	return 0;

}

/// <summary>
///  This conducts a single named pipe IPC with the server process
///  that is currently opening a named pipe which name is pipeName.
///  When user types a filename, it sends an IPC request to the server.
///  Server reads the file and send the file content through the named pipe to the client.
///  If there is no accessible file that the user requested, an error message will be returned instead.
/// </summary>
void doNamedPipelineIPCClientTask() {

	HANDLE hPipe;
	TCHAR readDataBuffer[BUFFER_SIZE + 1]; // including NULL termination
	LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");

	while (TRUE) {
		// Create the pipe
		hPipe = CreateFile(
			pipeName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY) {
			_tprintf(_T("Could not open the pipe properly. Error code: %d\n"), GetLastError());
			return 0;
		}

		if (!WaitNamedPipe(pipeName, 20000)) {
			_tprintf(_T("Could not open the pipe properly. Error code: %d\n"), GetLastError());
			return 0;
		}
	}

	DWORD pipeMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;
	BOOL isNamedPipeStatusChangeSuccessful = SetNamedPipeHandleState(
		hPipe,
		&pipeMode,
		NULL,
		NULL);

	if (!isNamedPipeStatusChangeSuccessful) {
		_tprintf(_T("Failed to change the pipe status. Error code: %d\n"), GetLastError());
		return 0;
	}

	_tprintf(_T("Filename to view the content> "));
	TCHAR fileName[MAX_PATH];
	_tscanf(_T("%s"), fileName);

	DWORD bytesWritten = 0;

	BOOL isWritingOnNamedPipeSuccessful = WriteFile(
		hPipe,
		fileName,
		(_tcslen(fileName) + 1) * sizeof(TCHAR),
		&bytesWritten,
		NULL);
	if (!isWritingOnNamedPipeSuccessful) {
		_tprintf(_T("Failed to write data on the named pipe. Error code: %d\n"), GetLastError());
		return 0;
	}

	DWORD bytesRead = 0;
	_fputts(_T("========== Start of the content ==========\n"), stdout);
	while (TRUE) {
		memset(readDataBuffer, 0, sizeof(readDataBuffer));
		BOOL isReadingDataFromNamedPipeSuccessful = ReadFile(
			hPipe,
			readDataBuffer,
			BUFFER_SIZE * sizeof(TCHAR),
			&bytesRead,
			NULL);

		if (!isReadingDataFromNamedPipeSuccessful && GetLastError() != ERROR_MORE_DATA)
			break;

		// If there is more than one byte read from the server,
		// proper NULL termination is encouraged.
		if (bytesRead > 0) {
			readDataBuffer[bytesRead] = 0;
			_tprintf(_T("%s"), readDataBuffer);
		}

		// Check for the end of the pipe stream.
		// If there is no more byte from the pipe, it's a signal of termination
		if (isReadingDataFromNamedPipeSuccessful && bytesRead == 0) {
			break;
		}
	}
	_fputts(_T("\n========== End of the content ==========\n\n"), stdout);

	CloseHandle(hPipe);
	return 0;

}