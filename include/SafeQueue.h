#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

// Thread-safe queue implementation
// Inspired by https://github.com/K-Adam/SafeQueue/blob/master/SafeQueue.hpp

template <typename T>
class SafeQueue
{
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond;

public:
    // Push an item into the queue
    void push(T item)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(item);
        }
        cond.notify_one();
    }

    // Pop an item from the queue; blocks if the queue is empty
    bool pop(T& outItem)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [this]
        {
            return !queue.empty();
        });
        outItem = queue.front();
        queue.pop();
        return true;
    }

    // Check if the queue is empty
    bool empty()
    {
        std::unique_lock<std::mutex> lock(mtx);
        return queue.empty();
    }

};
