// std::lock_guard in standard C++ is corresponded in critical section sync method in WINAPI C,
// often represented by EnterCriticalSection() and LeaveCriticalSection().
#include <iostream>
#include <thread>
#include <mutex>

const int maximumThreadQty = 6;

std::mutex mutex;
int totalCount = 0;

// Hitting critical section
void increaseTotalCount() {
    std::lock_guard<std::mutex> lock(mutex);  // Lock the mutex using std::lock_guard
    totalCount++;
    // lock_guard automatically unlocks the mutex when it goes out of scope
}

// Thread trying to call the problematic function about the critical section
void threadFunction() {
    for (int i = 0; i < 10000; i++)
        increaseTotalCount();
}

int main() {
    std::thread threads[maximumThreadQty];

    for (int index = 0; index < maximumThreadQty; index++) {
        threads[index] = std::thread(threadFunction);
    }

    for (int index = 0; index < maximumThreadQty; index++) {
        if (threads[index].joinable()) {
            threads[index].join();
        }
    }

    std::cout << "Total count: " << totalCount << std::endl;

    return 0;
}
