// PartAdder

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

/// <summary>
///  PartAdder.c will receive two different parameters
///  and return the sum from argv[1] to argv[2]
/// </summary>
/// <param name="argc">the number of execution parameters</param>
/// <param name="argv">the array of execution parameters(should be 3, 1 is itself, 2 are others)</param>
/// <returns></returns>
int _tmain(int argc, TCHAR* argv[]) {
	if (argc != 3) {
		_fputts(_T("Use only two explicit execution parameters.\n"), stdout);
		return -1;
	}

	DWORD start = _ttoi(argv[1]);
	DWORD end = _ttoi(argv[2]);
	DWORD total = 0;

	for (DWORD i = start; i <= end; i++)
		total += i;

	return total;
}