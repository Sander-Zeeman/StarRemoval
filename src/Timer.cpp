#include "../include/Timer.h"

Timer::Timer()
{
    start = clock();
}

Timer::~Timer()
{
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << std::setprecision(5) << elapsed << " seconds.";
}