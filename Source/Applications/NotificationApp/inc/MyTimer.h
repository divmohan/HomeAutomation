/**
 * @file
 * Defines the timer class
 */

#ifndef INC_MYTIMER_H_
#define INC_MYTIMER_H_

#include <time.h>
#include <signal.h>
#include <vector>
#include "EventList.h"

#define SIG SIGRTMIN

void TimerHandler (int signo);

class MyTimer
{
	timer_t MytimerId;

public:
	int CreateTimer(long durationSeconds, long durationNanoseconds);
    void AddEntitiesToBeNotified(EventList* event);
	int ClearTimer();
};

#endif
