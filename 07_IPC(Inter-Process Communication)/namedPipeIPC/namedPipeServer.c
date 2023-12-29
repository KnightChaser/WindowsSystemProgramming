// Named pipe server (IPC)
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUFFER_SIZE 1024

int ReadFileContentAndReturnToClient(HANDLE);

int _tmain(int argc, TCHAR* argv[]) {

	_fputts(_T("Named pipe server(IPC)\n"), stdout);

	LPTSTR pipeName = _T("\\\\.\\pipe\\simple_pipe");
	HANDLE hPipe;

	while (TRUE) {
		hPipe = CreateNamedPipe(
			pipeName,													// pipe name
			PIPE_ACCESS_DUPLEX,											// pipe communication mode(bidirectional)
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,		// text based communication
			PIPE_UNLIMITED_INSTANCES,									// allow to create as much pipes as possible
			BUFFER_SIZE,												// output buffer size (write)
			BUFFER_SIZE,												// input buffer size (read)
			20000,														// timeout (msec)
			NULL														// default security attribute
		);

		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(_T("Failed to create a named pipe. Error code: %d\n"), GetLastError());
			return -1;
		}

		BOOL isNamedPipeConnectionSuccessful = ConnectNamedPipe(hPipe, NULL);
		if (isNamedPipeConnectionSuccessful)
			ReadFileContentAndReturnToClient(hPipe);
		else {
			_fputts(_T("Failed to establish a connection with the named pipe\n"), stdout);
			_tprintf(_T("An error code obtained: %d\n"), GetLastError());
			CloseHandle(hPipe);
		}
	}

	return 1;
}

int ReadFileContentAndReturnToClient(HANDLE hPipe) {

	TCHAR fileName[MAX_PATH];
	TCHAR dataBuffer[BUFFER_SIZE];
	BOOL isReadingFileSuccessful;
	DWORD fileNameSize;

	// Reading user data from the named pipe
	isReadingFileSuccessful = ReadFile(
		hPipe,							// pipe handle
		fileName,						// read buffer (will receive a string of filename)
		MAX_PATH * sizeof(TCHAR),		// read buffer size
		&fileNameSize,					// received data size
		NULL
	);

	if (!isReadingFileSuccessful || fileNameSize == 0) {
		_tprintf(_T("Failed to read message from the pipe! Error code: %d\n"), GetLastError());
		return -1;
	}

	FILE* filePointer = _tfopen(fileName, _T("rt, ccs=UTF-8"));
	// In case of the requested file doesn't exist
	if (filePointer == NULL) {
		_tprintf(_T("Failed to open the file. Error code: %d\n"), GetLastError());
		return -1;
	}

	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;

	// Read file until FEOF(File EOF indicator)
	while (!feof(filePointer)) {

		bytesRead = fread(dataBuffer, 1, BUFFER_SIZE, filePointer);

		WriteFile(
			hPipe,			// pipe handle
			dataBuffer,		// the buffer to send through the pipe
			bytesRead,		// the size of the buffer
			&bytesWritten,	// the size of the data that will be sent through the pipe
			NULL
		);

		// File transmission seems to be inaccurate.
		if (bytesRead != bytesWritten) {
			_tprintf(_T("Failed to write data on the pipe! Error code: %d\n"), GetLastError());
			break;
		}

	}

	// Data transmission finished, clean up the current pipe setting
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	return 1;

}