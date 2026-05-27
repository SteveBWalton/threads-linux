/// Implmentation for the ShowNumber class.
/// Class to represent a class with an execute function.
///@file show_number.cpp

// File Header.
#include "show_number.h"

// System Headers.
#include <iostream>
#include <thread>
#include <mutex>


// Mutex for the execute() function.
std::mutex _mutexShowNumberClass;


// Class constructor.
// Initialise the number of this object.
ShowNumber::ShowNumber
(
    int number
)
{
    _number = number;
}



// Class destructor.
ShowNumber::~ShowNumber()
{
    /*
    // Wait for all showNumber functions to write.
    std::unique_lock <std::mutex> lock(_mutexShowNumberClass);

    std::cout << "D" << _number << " ";
    std::cout.flush();

    // Signal write is complete.
    lock.unlock();
    */
}



void ShowNumber::pause()
{
    // Wait for 1 second.
    std::this_thread::sleep_for(std::chrono::seconds(1));
}



void ShowNumber::execute()
{
    // Sleep / do work.
    pause();

    // Wait for all showNumber functions to write.
    std::unique_lock <std::mutex> lock(_mutexShowNumberClass);

    // Write the number.
    std::cout << _number << " ";
    std::cout.flush();

    // Signal write is complete.
    lock.unlock();

    // Sleep / do work.
    pause();

    // Wait for all the showNumber functions to write.
    lock.lock();

    // Write the number.
    std::cout << _number << " ";
    std::cout.flush();
}
