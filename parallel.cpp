/// Implmentation for the Parallel class.
///@file parallel.cpp

// File Header.
#include "parallel.h"

// System Headers.
#include <iostream>
#include <thread>
#include <functional>
#include <mutex>

// Application Headers.
#include "thread_pool.h"



// Mutex for the showNumber() function.
std::mutex mutexShowNumber_;


void pause()
{
    // Wait for 1 second.
    std::this_thread::sleep_for(std::chrono::seconds(1));
}



/// Display a number and then wait for one second.
void showNumber
(
    int number  ///< Specifies the number to display.
)
{
    // Sleep / do work.
    pause();

    // Wait for all showNumber functions to write.
    std::unique_lock <std::mutex> lock(mutexShowNumber_);

    // Write the number.
    std::cout << number << " ";
    std::cout.flush();

    lock.unlock();

    // Sleep / do work.
    pause();

    // Wait for all the showNumber functions to write.
    lock.lock();

    // Write the number.
    std::cout << number << " ";
    std::cout.flush();
}



/// Compare a multi-threaded loop with a serial loop.
int main
(
    int             argc,
    const char**    argv
)
{
    const int NUM_LOOPS = 30;

    // Welcome message.
    std::cout << "Hello from Parallel." << std::endl;

    // Call the function n times in serial.
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        showNumber(i);
    }
    std::cout << std::endl;

    // Call the function n times on a new thread each time.
    std::thread* myThreads[NUM_LOOPS];
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        myThreads[i] = new std::thread(showNumber, i);
    }

    // Wait for each thread to finish.
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        // Wait for this thread to finish.
        myThreads[i]->join();
    }
    std::cout << std::endl;

    // Call the function n times with thread pool object.
    ThreadPool threadPool(4);
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(showNumber, i));
    }
    threadPool.waitAllFinish();
    threadPool.waitAllFinish();
    std::cout << std::endl;

    // Lets go again with the thread pool.
    threadPool.stopMultithreading();
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(showNumber, i));
    }
    threadPool.waitAllFinish();
    std::cout << std::endl;

    // Lets go again with the thread pool.
    threadPool.startMultithreading();
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(showNumber, i));
    }
    threadPool.waitAllFinish();
    std::cout << std::endl;

    // Return success.
    std::cout << "Goodbye from Parallel." << std::endl;
    return 0;
}

