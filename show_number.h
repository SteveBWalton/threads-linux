/// Definitions for the ShowNumber class.
///@file show_number.h

#pragma once

// Class to write an index.
class ShowNumber
{
public:
    ShowNumber(int);
    ~ShowNumber();

private:
    // The number for this instance.
    int _number;

private:
    // Pause for 1 second.
    void pause();

public:
    // Run the class.
    void execute();
};
