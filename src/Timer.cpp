#include "../include/Timer.h"

TimerWrapper* TimerWrapper::m_wrapper = nullptr;

TimerWrapper* TimerWrapper::TimerInstance()
{
   if (m_wrapper == nullptr)
       m_wrapper = new TimerWrapper();

   return m_wrapper;
}

void TimerWrapper::startTimer()
{
    timers.push(new Timer());
}

void TimerWrapper::stopTimer(const char *action)
{
    std::cout << action << " took ";
    delete timers.top();
    timers.pop();
    std::cout << std::endl;
}

void TimerWrapper::cleanTimer()
{
    if (timers.size() == 0) {
        delete m_wrapper;
        m_wrapper = nullptr;
    } else {
        std::cout << "Can't free the timers while a timer is still running!" << std::endl;
    }
}
