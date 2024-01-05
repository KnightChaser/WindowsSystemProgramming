// Standard C++ implementaion of stringStdIOEventHandlingEventMutexMix.c
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

struct SynchEventMutexObj {
    std::string data;
    std::condition_variable condition;
    std::mutex mutex;
};

void outputThreadFunction(SynchEventMutexObj& syncObj);
void countThreadFunction(SynchEventMutexObj& synbObj);


int main() {
    SynchEventMutexObj syncObj;

    // Launch threads
    std::thread outputThread(outputThreadFunction, std::ref(syncObj));
    std::thread countThread(countThreadFunction, std::ref(syncObj));

    // User input
    std::cout << "Insert string> ";
    std::getline(std::cin, syncObj.data);

    // Notify threads that data is ready (such like event)
    // Suspend all threads until the signal (resume threads after user input)
    syncObj.condition.notify_all();

    // Wait for threads to finish
    outputThread.join();
    countThread.join();

    return 0;
}

void outputThreadFunction(SynchEventMutexObj& syncObj) {
    // Wait until condition is notified
    std::unique_lock<std::mutex> lock(syncObj.mutex);
    syncObj.condition.wait(lock);

    // Process data (shared object, protected by Mutex)
    std::cout << "Output string: " << syncObj.data << std::endl;
}

void countThreadFunction(SynchEventMutexObj& syncObj) {
    // Wait until condition is notified
    std::unique_lock<std::mutex> lock(syncObj.mutex);
    syncObj.condition.wait(lock);

    // Process data (shared object, ,protected by Mutex)
    std::cout << "Output string length: " << syncObj.data.length() << std::endl;
}