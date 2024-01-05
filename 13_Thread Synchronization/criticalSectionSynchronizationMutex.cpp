#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

const unsigned int maximumThreadQty = 6;

std::mutex mutex;
unsigned int totalCount = 0;

void increaseTotalCount() {
    mutex.lock();   // Mutex lock
    totalCount++;   // ** Critical section **
    mutex.unlock(); // Mutex unlock
}

void threadFunction() {
    for (unsigned int i = 0; i < 10000; i++)
        increaseTotalCount();
}

int main() {

    // Mutex object in standard C++
    std::mutex mutex;

    // Create a thread array amount of threads
    std::vector<std::thread> threads;

    // Create a thread pool and execute parallel tasks
    for (int index = 0; index < maximumThreadQty; index++) {
        threads.push_back(std::thread(threadFunction));
    }

    // Wait until the created threads finish their workings
    for (int index = 0; index < maximumThreadQty; index++) {
        if (threads[index].joinable()) {
            threads[index].join();
        }
    }

    std::cout << "Total count: " << totalCount << std::endl;

    return 0;
}
