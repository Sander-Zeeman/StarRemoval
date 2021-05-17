#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <iostream>
#include <iomanip>

class Timer
{

public:
    Timer();
    ~Timer();

private:
    time_t start, end;
    double elapsed;

};

#endif