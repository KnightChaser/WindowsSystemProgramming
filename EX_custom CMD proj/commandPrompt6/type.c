// type.c -> type.exe

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

/// <summary>
///  Read file located in "TCHAR* filepath" and save the content to the fileContent
///  (TCHAR array), and return the array as a ptr
/// </summary>
/// <param name="filepath">The filepath that the user wants to read</param>
/// <returns></returns>
void readFileContent(const TCHAR* filepath) {
	FILE* targetFilePtr = _tfopen(filepath, _T("rt"));
	const UINT bufferReadSize = 4096;

	if (targetFilePtr == NULL) {
		_tprintf(_T("Error opening file: %s\n"), filepath);
		return;
	}

	TCHAR* bufferRead = (TCHAR*)malloc(bufferReadSize * sizeof(TCHAR));

	if (bufferRead == NULL) {
		_tprintf(_T("Memory allocation failed during reading the file %s, obtained an error code %d\n"),
			filepath, GetLastError());
		fclose(targetFilePtr);
		return;
	}

	while (_fgetts(bufferRead, bufferReadSize, targetFilePtr)) {
		_fputts(bufferRead, stdout);
	}

	free(bufferRead);
	fclose(targetFilePtr);
}

// ./type.exe openAndReadThisFile.txt
int _tmain(int argc, TCHAR* argv[]) {
	if (argc < 2)
		// too few argument
		return -1;

	readFileContent(argv[1]);

	return 0;
}