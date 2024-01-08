// Thread pooling small examples
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define	workMaxQty		10000
#define threadMaxQty	50

typedef void (*WORK) (void);	// A working function that the threads in the pool will work with

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadToPool(DWORD threadQty);
void WorkerThreadFunction(LPVOID pParam);

// A single unit thread for tasking
typedef struct __WorkerThread {
	HANDLE hThread;
	DWORD idThread;
} WorkerThread;

// A struct that manages works and threads (thread pool)
struct __ThreadPool {
	WORK workList[workMaxQty];						// An array to register the work
	WorkerThread workerThreadList[threadMaxQty];	// Workable thread information
	HANDLE workerEventList[threadMaxQty];			// An event to control threads' wakeup/sleep status
	DWORD currentWorkIndex;							// current work (the index number of work to be executed at first)
	DWORD lastAddedWorkIndex;						// last work (the index number of work to be executed at last)
	DWORD threadIndex;								// A number of available thread numbers in the pool
} gThreadPool;

// Wrapper functions related to consistency protection to gThreadPool via MUTEX
static HANDLE hThreadPoolMutex = NULL;

void InitializeMutexObject(HANDLE* hMutex) {
	*hMutex = CreateMutex(NULL, FALSE, NULL);
}

void DeleteMutexObject(HANDLE* hMutex) {
	BOOL isMutexClosed = CloseHandle(*hMutex);
	if (!isMutexClosed)
		_tprintf(_T("Mutex is not closed at DeleteMutexObject(), obtained an error code: %d\n"), GetLastError());
}

void AcquireMutexObject(HANDLE* hMutex) {
	DWORD isWaitingSuccessful = WaitForSingleObject(*hMutex, INFINITE);
	if (isWaitingSuccessful == WAIT_FAILED)
		_tprintf(_T("Waitng for MUTEX object at AcquireMutexObject() failed, obtained an error code: %d\n"), GetLastError());
}

void ReleaseMutexObject(HANDLE* hMutex) {
	BOOL isMutexReleased = ReleaseMutex(*hMutex);
	if (!isMutexReleased)
		_tprintf(_T("Releasing MUTEX object at ReleaseMutexObject() failed, obtained an error code: %d\n"), GetLastError());
}


// A function that registers work to the thread pool
DWORD AddWorkToPool(WORK work) {
	AcquireMutexObject(&hThreadPoolMutex);
	if (gThreadPool.lastAddedWorkIndex >= workMaxQty) {
		_fputts(_T("AddWorkToPool() failed because # of work exceeded the threshold workMaxQty\n"), stdout);
		return NULL;
	}

	// Register work
	gThreadPool.workList[gThreadPool.lastAddedWorkIndex++] = work;

	// After finishing registering work,
	// Waking up every sleeping thread to initiate work. (Actually, no need to waking up every thread in real)
	for (DWORD index = 0; index < gThreadPool.threadIndex; index++)
		SetEvent(gThreadPool.workerEventList[index]);

	ReleaseMutexObject(&hThreadPoolMutex);
	return 1;
}

// A function that called when bring Work from Thread
// Defined to protect access to gThreadPool
WORK GetWorkFromPool() {
	WORK work = NULL;
	AcquireMutexObject(&hThreadPoolMutex);

	// If there is no work to task
	if (!(gThreadPool.currentWorkIndex < gThreadPool.lastAddedWorkIndex)) {
		ReleaseMutexObject(&hThreadPoolMutex);
		return NULL;
	}

	work = gThreadPool.workList[gThreadPool.currentWorkIndex++];
	ReleaseMutexObject(&hThreadPoolMutex);
	return work;	// Get work ptr address from the pool
}

// Create a thread pool
DWORD MakeThreadToPool(DWORD threadQty) {
	InitializeMutexObject(&hThreadPoolMutex);
	DWORD capacity = workMaxQty - gThreadPool.threadIndex;
	if (capacity < threadQty)
		threadQty = capacity;

	for (DWORD index = 0; index < threadQty; index++) {
		DWORD idThread;
		HANDLE hThread;

		gThreadPool.workerEventList[gThreadPool.threadIndex]
			= CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = CreateThread(
			NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction,
			(LPVOID)gThreadPool.threadIndex, 0, &idThread);
		gThreadPool.workerThreadList[gThreadPool.threadIndex].hThread = hThread;
		gThreadPool.workerThreadList[gThreadPool.threadIndex].idThread = idThread;
		gThreadPool.threadIndex++;
	}
	return threadQty;
}

// A worker thread
void WorkerThreadFunction(LPVOID pParam) {
	WORK workFunction;
	HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];
	
	while (TRUE) {
		workFunction = GetWorkFromPool();
		if (workFunction == NULL) {
			// Set this thread to be blocked until the work is allocated to this
			WaitForSingleObject(event, INFINITE);
			continue;
		}
		workFunction();
	}
}

static HANDLE hTestFunctionMutex;

// A simple work function that will be the major task of this thread pool
void TestFunction() {

	// Variable index need mutex protection actually because multiple objects trying to access this.
	// This protective approach will make the procedure entirely safe.
	AcquireMutexObject(&hTestFunctionMutex);
	static UINT index = 0;
	index++;
	_tprintf(_T("ThreadPoolTest: %5d, Processing thread ID: %6d\n"), index, GetCurrentThreadId());
	ReleaseMutexObject(&hTestFunctionMutex);
	
}

int _tmain(int argc, TCHAR* argv[]) {
	
	// Create a thread pool
	MakeThreadToPool(10);

	InitializeMutexObject(&hTestFunctionMutex);

	// Register multiple work
	for (UINT index = 0; index < 1000; index++)
		AddWorkToPool(TestFunction);

	// Just wait until the thread finishes working
	TCHAR ex = _gettchar();

	DeleteMutexObject(&hTestFunctionMutex);

	return 0;
}