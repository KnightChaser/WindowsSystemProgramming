// Get file size (32 bit system / 64 bit system)
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[]) {
	TCHAR filename[] = _T("data.txt");
	HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE) {
		_tprintf(_T("File creation fault. Obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	// 32 bit style
	DWORD high4ByteFileSize = 0;
	DWORD low4ByteFileSize = GetFileSize(hFile, &high4ByteFileSize);

	_tprintf(_T("High 4B file size: %9u B\n"), high4ByteFileSize);
	_tprintf(_T(" Low 4B file size: %9u B\n"), low4ByteFileSize);

	// 64 bit style
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);
	_tprintf(_T("  Total file size: %9lld B\n"), fileSize.QuadPart);

	CloseHandle(hFile);
	return 0;
}