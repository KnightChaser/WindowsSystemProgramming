// A simulation of busy crowded restaurants
// (A little bit complicated) semaphore example

#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <Windows.h>
#include <process.h>

#define maximumCustmerQty 50
#define randomValueGenerationMinimum 10
#define randomValueGenerationMaximum 50
#define tableQty 10

HANDLE hSemaphore;
DWORD randomOccupationTimeArray[50];
UINT currentSeatQty = tableQty;

// Custom wrapper function of acquiring Semaphore
void AcquireSemaphore(HANDLE hSemaphore) {
	WaitForSingleObject(hSemaphore, INFINITE);
}

void clientHavingMeal(DWORD time) {
	AcquireSemaphore(hSemaphore, INFINITE);
	// start of the critical section ...
	_tprintf(_T("[Seat: %02d/%d] Customer #%d entered.\n"), currentSeatQty, tableQty, GetCurrentThreadId());
	InterlockedDecrementAcquire(&currentSeatQty);
	_tprintf(_T("[Seat: %02d/%d] Customer #%d is having meal...\n"), currentSeatQty, tableQty, GetCurrentThreadId());
	
	Sleep(100 * time); // the client is meanting the ordered meal...

	ReleaseSemaphore(hSemaphore, 1, NULL);
	// end of the critical section ...
	InterlockedIncrementAcquire(&currentSeatQty);

	_tprintf(_T("[Seat: %02d/%d] Customer #%d exited.\n"), currentSeatQty, tableQty, GetCurrentThreadId());
}

UINT WINAPI ThreadFunction(LPVOID lpParam) {
	clientHavingMeal((DWORD)lpParam);
	return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
	DWORD dwThreadIDs[maximumCustmerQty];	// Identifier of the each customer entering the restaurant.
	HANDLE hThreads[maximumCustmerQty];		// Each thread represents each customer entering the restaurant for the meal.
	srand((unsigned)time(NULL));			// Initialize randomization vectors (rand seed)

	// Each customer will spend k time of having meal. (K is a random value within the predefined range)
	for (DWORD index = 0; index < maximumCustmerQty; index++)
		randomOccupationTimeArray[index] = (DWORD)((double)rand() / (double)RAND_MAX) * randomValueGenerationMaximum + randomValueGenerationMinimum;

	// Create a semaphore
	hSemaphore = CreateSemaphore(
		NULL,
		tableQty,
		tableQty,
		NULL
	);

	if (hSemaphore == NULL) {
		_tprintf(_T("Failed to create the semaphore, obtained an error code: %d\n"), GetLastError());
		return -1;
	}

	// Create threads meaning customers
	for (DWORD index = 0; index < maximumCustmerQty; index++) {
		hThreads[index] = (HANDLE)_beginthreadex(
			NULL, 0, ThreadFunction, (void*)randomOccupationTimeArray[index],
			CREATE_SUSPENDED, (unsigned*)&dwThreadIDs[index]);
		if (hThreads[index] == NULL) {
			_tprintf(_T("Failed to create thread #%d, obtained an error code: %d\n"), index, GetLastError());
			return -1;
		}
	}

	// Initiate blocked threads
	for (DWORD index = 0; index < maximumCustmerQty; index++)
		ResumeThread(hThreads[index]);

	WaitForMultipleObjects(maximumCustmerQty, hThreads, TRUE, INFINITE);

	_fputts(_T("========== Finished ==========\n"), stdout);

	for (DWORD index = 0; index < maximumCustmerQty; index++)
		CloseHandle(hThreads[index]);
	CloseHandle(hSemaphore);	// Semaphore is also a kernel object that needs CloseHandle() at the end. Don't forget!

	return 0;
}