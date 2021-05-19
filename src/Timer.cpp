#include "../include/Timer.h"

TimerWrapper* TimerWrapper::m_wrapper = nullptr;

TimerWrapper* TimerWrapper::TimerInstance()
{
   if (m_wrapper == nullptr)
       m_wrapper = new TimerWrapper();

   return m_wrapper;
}

void TimerWrapper::startTimer() {
    timers.push(new Timer());
}

void TimerWrapper::stopTimer(const char *action) {
    std::cout << action << " took ";
    Timer *t = timers.top();
    delete t;
    timers.pop();
    std::cout << std::endl << std::endl;
}
