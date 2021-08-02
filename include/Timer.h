#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <iomanip>
#include <iostream>
#include <stack>

class Timer
{
public:
    Timer() { m_start = clock(); }
    ~Timer() {
        m_end = clock();
        m_elapsed = (double)(m_end - m_start) / CLOCKS_PER_SEC;
        std::cout << std::setprecision(5) << m_elapsed << " seconds.";
    }

private:
    time_t m_start, m_end;
    double m_elapsed;
};

class TimerWrapper
{
public:
    TimerWrapper() {}
    ~TimerWrapper() {}

    static TimerWrapper *TimerInstance();

    void startTimer();
    void stopTimer(const char *action);
    void cleanTimer();

private:
    static TimerWrapper *m_wrapper;

    std::stack<Timer*> timers;
};

#endif
