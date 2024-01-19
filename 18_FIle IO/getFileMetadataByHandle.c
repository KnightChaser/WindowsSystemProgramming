// Obtaining file information via handle
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LENGTH_BYTE 100

void ShowAttributes(DWORD attributes);
_TCHAR* ShowFileTime(FILETIME t);

int _tmain(int argc, _TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");
	BY_HANDLE_FILE_INFORMATION fileInformationStruct;

	HANDLE hFile = CreateFile(
		filename, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(_T("CreateFile() function got fault, obtained an error code: %d\n"), GetLastError());
		return 0;
	}

	GetFileInformationByHandle(hFile, &fileInformationStruct);
	ShowAttributes(fileInformationStruct.dwFileAttributes);

	_tprintf(_T("File size: %d B\n"), fileInformationStruct.nFileSizeLow);
	_TCHAR* creationTime = ShowFileTime(fileInformationStruct.ftCreationTime);
	_tprintf(_T("File creation time    : %s\n"), creationTime);
	free(creationTime);

	_TCHAR* accessTime = ShowFileTime(fileInformationStruct.ftLastAccessTime);
	_tprintf(_T("File last access time : %s\n"), accessTime);
	free(accessTime);

	_TCHAR* writeTime = ShowFileTime(fileInformationStruct.ftLastWriteTime);
	_tprintf(_T("File last write time  : %s\n"), writeTime);
	free(writeTime);

	CloseHandle(hFile);
	return 0;
}

void ShowAttributes(DWORD attributes) {
	if (attributes & FILE_ATTRIBUTE_NORMAL)
		_fputts(_T("Normal\n"), stdout);
	else {
		if (attributes & FILE_ATTRIBUTE_READONLY)
			_fputts(_T("Readonly(RDONLY)\n"), stdout);
		if (attributes & FILE_ATTRIBUTE_HIDDEN)
			_fputts(_T("Hidden\n"), stdout);
	}
}

_TCHAR* ShowFileTime(FILETIME t) {
	TCHAR* fileTimeInformation = (_TCHAR*)malloc(STRING_LENGTH_BYTE * sizeof(_TCHAR));

	if (fileTimeInformation == NULL) {
		_tprintf(_T("Memory allocation error\n"));
		exit(EXIT_FAILURE);  // or handle the error in an appropriate way
	}

	FILETIME ft = t;
	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&ft, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	_stprintf(fileTimeInformation, _T("[%02d/%02d/%d %02d:%02d]"),
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	return fileTimeInformation;
}
