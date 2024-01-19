// Referring and modifying file atributes
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void ShowAttributes(DWORD attributes);

int _tmain(int argc, TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");

	_fputts(_T("Original file attributes...\n"), stdout);
	DWORD attributes = GetFileAttributes(filename);
	ShowAttributes(attributes);

	attributes |= (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributes(filename, attributes);

	_fputts(_T("Modified file attributes...\n"), stdout);
	attributes = GetFileAttributes(filename);
	ShowAttributes(attributes);

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