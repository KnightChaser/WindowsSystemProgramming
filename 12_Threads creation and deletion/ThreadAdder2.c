// ThreadAdder2.c
// Race condition happens to the global variables... but anyway.
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

static int total = 0;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {

	DWORD* parameterNumberPtr = (DWORD*)lpParam;
	DWORD x = *parameterNumberPtr;
	DWORD y = *(parameterNumberPtr + 1);

	// sum (i:x->y)
	for (DWORD i = x; i <= y; i++)
		total += i;		// global variable (THIS APPROACH IS NOT THREAD SAFE!)

	return 0;			// Just finish
}

int _tmain(int argc, TCHAR* argv[]) {

	DWORD dwThreadID[3];
	HANDLE hThread[3];
	DWORD parameterThread[] = { 1, 3, 4, 7, 8, 10 };

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

	// No need to run GetExitCodeThread because the given 3 threads apply their results to the global variable
	_tprintf(_T("sum(1->10): %d\n"), total);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	CloseHandle(hThread[2]);

	return 0;

}