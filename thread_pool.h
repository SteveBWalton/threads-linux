/// Definitions for the ThreadPool class.
///@file thread_pool.h

#pragma once

// System Headers.
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>



// Class to represent a collection of threads that execute tasks from a queue.
class ThreadPool
{
public:
    ThreadPool(int);
    ~ThreadPool();

private:
    // Mutex for the add task function.
    std::mutex _mutexAddTask;

    // Condition to signal task in the queue.
    std::condition_variable _condAddTask;

    // True when the class is closing and should stop feeding tasks to the threads.
    bool _isShutdown;

    // The queue of tasks to feed to the threads.
    std::queue <std::function <void(void)>> _tasks;

    // The threads to actually execute the tasks on.
    std::vector <std::thread> _threads;

    // The number of tasks that are running and in the queue.
    // This is atomic for operations like numTasksPending_++, numTasksPending_-- and numTasksPending_+= 1.
    // But not for numTasksPending_ = numTasksPending_ + 1.
    // The atomic means that threads can write and read to the variable without mutex protection.
    std::atomic<int> _numTasksPending;

    // Mutex for the count tasks calculation.
    std::mutex _mutexNumTasks;

    // Condition to wait for numTasksPending_ to reach zero.
    std::condition_variable _condNumTasksZero;

    // False to stop multithreading.  Can be used for debuging.
    bool _isMultithreading;

private:
    // The threads execute this function.
    void threadEntry(int);

public:
    // Add a task to the queue for the thread pool to execute.
    void addTask(std::function <void(void)>);
    // Wait until the queue is empty and the running tasks complete.
    void waitAllFinish();
    // Stop using threads.  Might be usefult for debugging.
    void stopMultithreading();
    // Start using threads.  The default mode.
    void startMultithreading();
};
