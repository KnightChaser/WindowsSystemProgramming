// An example of exit handler
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int readStringAndWrite(void);

int _tmain(int argc, TCHAR* argv[]) {
	int state = 0;
	while (TRUE) {
		state = readStringAndWrite();
		if (state == -1) {
			_fputts(_T("some problem occured!\n"), stdout);
			break;
		}
		if (state == 0) {
			_fputts(_T("sucessful exit!\n"), stdout);
			break;
		}
	}
	return 0;
}

int readStringAndWrite(void) {
	FILE* filePtr = NULL;
	TCHAR* stringBufferPtr = NULL;

	__try {
		filePtr = _tfopen(_T("string.dat"), _T("a+t"));
		if (filePtr == NULL)
			return -1;
		DWORD stringLength = 0;
		_fputts(_T("Input string length, 0 to exit > "), stdout);
		_tscanf(_T("%d"), &stringLength);

		// Suppose that input "0" is a valid termination.
		if (stringLength == 0)
			return 0;

		stringBufferPtr = (TCHAR*)malloc((stringLength + 1) * sizeof(TCHAR));
		if (stringBufferPtr == NULL)
			return -1;

		_fputts(_T("Input string > "), stdout);
		_tscanf(_T("%s"), stringBufferPtr);
		_ftprintf(filePtr, _T("%s \n"), stringBufferPtr);		// write the buffer content to the file
	}
	__finally {
		// Even though there was a runtime error, closing dynamically allocated objects is encouraged.
		if (filePtr != NULL)
			fclose(filePtr);

		if (stringBufferPtr != NULL)
			fclose(stringBufferPtr);
	}

	return 1;
}