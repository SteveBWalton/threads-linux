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
#include "show_number.h"



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

    std::vector<ShowNumber> showNumbers;
    showNumbers.reserve(NUM_LOOPS);

    // Create and call the class in serial.
    std::cout << "In Sequence." << std::endl;
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        showNumbers.emplace_back(ShowNumber(i));
        showNumbers[i].execute();
    }
    std::cout << std::endl;

    // Call the function n times on a new thread each time.
    std::cout << "In a Thread object each." << std::endl;
    std::thread* myThreads[NUM_LOOPS];
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        myThreads[i] = new std::thread(&ShowNumber::execute, &showNumbers[i]);
    }

    // Wait for each thread to finish.
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        // Wait for this thread to finish.
        myThreads[i]->join();
    }
    std::cout << std::endl;

    // Call the function n times with thread pool object.
    std::cout << "In a ThreadPool(4) object." << std::endl;
    ThreadPool threadPool(4);
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(&ShowNumber::execute, &showNumbers[i]));
    }
    threadPool.waitAllFinish();
    // threadPool.waitAllFinish();
    std::cout << std::endl;

    // Lets go again with the thread pool.
    std::cout << "In a ThreadPool(4) object with multi-threading off." << std::endl;
    threadPool.stopMultithreading();
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(&ShowNumber::execute, &showNumbers[i]));
    }
    threadPool.waitAllFinish();
    std::cout << std::endl;

    // Lets go again with the thread pool.
    std::cout << "In a ThreadPool(4) object with multi-threading back on." << std::endl;
    threadPool.startMultithreading();
    for (int i = 0; i < NUM_LOOPS; i++)
    {
        threadPool.addTask(std::bind(&ShowNumber::execute, &showNumbers[i]));
    }
    threadPool.waitAllFinish();
    std::cout << std::endl;

    // Return success.
    std::cout << "Goodbye from Parallel." << std::endl;
    return 0;
}

