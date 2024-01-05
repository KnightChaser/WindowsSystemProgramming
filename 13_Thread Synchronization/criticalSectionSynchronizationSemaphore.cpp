// Since C++20, There is counting_semaphore in STL!
// Ref: https://developers.redhat.com/articles/2023/04/18/implementing-c20-semaphores
#include <iostream>
#include <vector>
#include <thread>
#include <semaphore>

const int MAX_THREADS = 5;
const int MAX_COUNT = 20;

// Set the maximum threshold for the semaphore, limiting the max # of threads accessing the critical section simultaneously.
std::counting_semaphore<MAX_THREADS> semaphore(MAX_THREADS);

void ThreadFunction(int threadNum) {
    for (int i = 0; i < MAX_COUNT; i++) {
        // Wait for the semaphore
        semaphore.acquire();

        // Critical section
        std::cout << "Thread " << threadNum << ": Count " << i << std::endl;

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Release the semaphore
        semaphore.release();
    }
}

int main() {
    std::vector<std::thread> threads;

    // Create worker threads
    for (int index = 0; index < MAX_THREADS; index++) {
        threads.emplace_back(ThreadFunction, index);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
