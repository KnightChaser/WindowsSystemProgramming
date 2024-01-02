// ThreadAdder1.c
// Examining the distribution of workload using threads
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

DWORD WINAPI ThreadFunction(LPVOID lpParam) {

	DWORD* parameterNumberPtr = (DWORD*)lpParam;
	DWORD x = *parameterNumberPtr;
	DWORD y = *(parameterNumberPtr + 1);
	DWORD total = 0;

	// sum (i:x->y)
	for (DWORD i = x; i <= y; i++)
		total += i;

	return total;
}

int _tmain(int argc, TCHAR* argv[]) {

	DWORD dwThreadID[3];
	HANDLE hThread[3];
	DWORD parameterThread[] = { 1, 3, 4, 7, 8, 10 };
	DWORD total = 0;		// the total value
	DWORD result = 0;		// each values comes from the independent threads

	for (DWORD threadIndex = 0; threadIndex < 3; threadIndex++) {
		// hThread[threadIndex] = CreateThread(ThreadFunction((LPVOID)&parameterThread, (LPVOID)&(parameterThread+1)))
		hThread[threadIndex] = CreateThread(NULL, 0, ThreadFunction, (LPVOID)(&parameterThread[2 * threadIndex]), 0, &dwThreadID[threadIndex]);
		if (hThread[threadIndex] == NULL) {
			_tprintf(_T("Thread #%d creation failed.\n"), threadIndex);
			return -1;
		}
	}

	WaitForMultipleObjects(
		3,				// Wait 3 kernel objects
		hThread,		// of the given thread(HANDLE array)
		TRUE,			// to be signaled
		INFINITE		// limitlessly
	);

	GetExitCodeThread(hThread[0], &result);	total += result;
	GetExitCodeThread(hThread[1], &result);	total += result;
	GetExitCodeThread(hThread[2], &result);	total += result;
	_tprintf(_T("sum(1->10): %d\n"), total);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	return 0;
	
}