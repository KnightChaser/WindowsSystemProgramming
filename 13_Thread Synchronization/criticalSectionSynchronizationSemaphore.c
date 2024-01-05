#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>

#define MAX_THREADS 5
#define MAX_COUNT 20

HANDLE semaphore;

DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    DWORD threadNum = (DWORD)lpParam;  // Change UINT to DWORD

    for (int i = 0; i < MAX_COUNT; i++) {
        // Wait for the semaphore
        WaitForSingleObject(semaphore, INFINITE);

        // Critical section
        printf("Thread %lu: Count %d\n", threadNum, i);  // Use %lu for DWORD

        // Release the semaphore
        ReleaseSemaphore(semaphore, 1, NULL);

        // Simulate some work
        Sleep(500);
    }

    return 0;
}

int _tmain(int argc, TCHAR* argv[]) {
    DWORD dwThreadId[MAX_THREADS];
    HANDLE hThread[MAX_THREADS];

    // Create a semaphore with an initial count of 1 and a maximum count of 5
    semaphore = CreateSemaphore(NULL, 1, MAX_THREADS, NULL);
    if (semaphore == NULL) {
        _tprintf(_T("Semaphore creation failed (%d)\n"), GetLastError());
        return 1;
    }

    // Create worker threads
    for (DWORD index = 0; index < MAX_THREADS; index++) {
        hThread[index] = CreateThread(NULL, 0, ThreadFunction, (LPVOID)index, 0, &dwThreadId[index]);  // Change unsigned to DWORD
        if (hThread[index] == NULL) {
            _tprintf(_T("Thread creation failed (%d)\n"), GetLastError());
            return 1;
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);

    // Close thread handles
    for (DWORD index = 0; index < MAX_THREADS; index++)
        CloseHandle(hThread[index]);

    // Close the semaphore handle
    CloseHandle(semaphore);

    return 0;
}
