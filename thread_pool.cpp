/// Implmentation for the TheadPool class.
/// Class to represent a pool of threads to execute functions.
///@file thread_pool.cpp

// File Header.
#include "thread_pool.h"

// System Headers.
#include <iostream> // cout() Remove this, debugging only.
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


// Class constructor.
// Initialise the specified number of threads.
ThreadPool::ThreadPool
(
    int numThreads
)
{
    // Create the specified number of threads.
    _threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++)
    {
        _threads.emplace_back(std::bind(&ThreadPool::threadEntry, this, i));
    }
    _numTasksPending = 0;
    _isMultithreading = true;
}



// Class destructor.
// Close all the threads.
ThreadPool::~ThreadPool()
{
    // Lock ends after this block.
    {
        // Wait for all add task functions to finish.
        std::unique_lock <std::mutex> lock(_mutexAddTask);

        // Tell the threads to stop.
        _isShutdown = true;
        _condAddTask.notify_all();
    }

    // Wait for all threads to stop.
    std::cout << "Joining threadpool threads." << std::endl;
    for (auto& thread : _threads)
    {
        thread.join();
    }
}



// Add a task to the queue.
void ThreadPool::addTask
(
    std::function <void(void)> newTask
)
{
    // Wait for all add task functions to finish.
    std::unique_lock <std::mutex> lock(_mutexAddTask);

    if (_isMultithreading)
    {
        // Another task before we are finished.
        _numTasksPending++;

        // Place the task on the quene.
        _tasks.emplace(std::move(newTask));

        // Unblock one thread.
        _condAddTask.notify_one();
    }
    else
    {
        // Simply do the task on this thread.
        newTask();
    }
}



// The function executed by the threads.
// Fetch a task from the queue and execute it.
// Otherwise wait for the queue to contains a task.
void ThreadPool::threadEntry
(
    int i
)
{
    // The next task taken from the task queue.
    std::function <void(void)> nextTask;

    // Loop until the isShutdown signal is set.
    while (1)
    {
        // Lock ends after this block.
        {
            // Wait for all the add task functions to finish.
            std::unique_lock <std::mutex> lock(_mutexAddTask);

            // Check if there are any tasks to do.
            while (!_isShutdown && _tasks.empty())
            {
                // Releases the add task lock and waits for its return.
                _condAddTask.wait(lock);
            }

            if (_tasks.empty())
            {
                // Sleep for 0.5 second, so we can we that the lock releases each thread in turn.  This is debugging only, not needed.
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                // No tasks to do, so we are shutting down.
                std::cout << "ThreadPool thread " << i << " terminates" << std::endl;

                // Close the thread.
                return;
            }

            // Get the next task.
            nextTask = std::move(_tasks.front());
            _tasks.pop();
        }
        // Release the add task lock.

        // Do the task without holding any locks.
        nextTask();

        // The task is finished.
        _numTasksPending--;
        if (_numTasksPending == 0)
        {
            // All jobs are finished.
            _condNumTasksZero.notify_all();
        }
    }
}



/// Wait for all the threads to finish.
void ThreadPool::waitAllFinish()
{
    // Wait for the num task functions to finish.
    std::unique_lock<std::mutex> lock(_mutexNumTasks);

    // Check for any pending tasks.
    if (_numTasksPending > 0)
    {
        // Wait for notification that numJobsPending_ has reached zero.
        _condNumTasksZero.wait(lock);
    }
}



/// Stop any threads in the pool from accepting new tasks.
void ThreadPool::stopMultithreading()
{
    // Wait for all add task functions to finish.
    std::unique_lock <std::mutex> lock(_mutexAddTask);

    // Disable the multithreading mode.
    _isMultithreading = false;
}



/// Allow the threads in the pool to accept new tasks.
void ThreadPool::startMultithreading()
{
    // Wait for all add task functions to finish.
    std::unique_lock <std::mutex> lock(_mutexAddTask);

    // Enable the multithreading mode.
    _isMultithreading = true;
}
