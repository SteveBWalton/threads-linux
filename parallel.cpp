/// Implmentation for the Parallel class.
///@file parallel.cpp

// File Header.
#include "parallel.h"

// System Headers.
#include <iostream>
#include <unistd.h>
#include <thread>


/// Display a number and then wait for one second.
void showNumber
(
    int number  ///< Specifies the number to display.
)
{
    std::cout << number << " ";
    std::cout.flush();

    // Wait for 1 second.
    usleep(1000000);
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

    // Return success.
    std::cout << "Goodbye from Parallel." << std::endl;
    return 0;
}

