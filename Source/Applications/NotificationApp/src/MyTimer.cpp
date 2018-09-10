#include <iostream>
#include "MyTimer.h"


//std::vector<EventList*>NotifyObjects;
EventList* NotifyObjects;

void TimerHandler (int signo)
{
      //remind
   // std::cout<<"Hi reminding ";


  //  NotifyObjects[0]->UpdateListOnTimerExpiry();
   NotifyObjects->UpdateListOnTimerExpiry();

}

int MyTimer::CreateTimer(long durationSeconds, long durationNanoseconds)
{
	struct sigevent sev;
	struct sigaction user_action;
	struct itimerspec itspec;
	user_action.sa_flags = SA_RESTART;
	user_action.sa_handler = TimerHandler;
	sigemptyset(&user_action.sa_mask);

	if ( sigaction(SIG,&user_action, NULL) == -1)
	{
		return -1;
	}
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo  = SIG;

	if ( timer_create(CLOCK_REALTIME,&sev, &MytimerId) == -1)
	{
		return -1;
	}

	itspec.it_value.tv_sec = durationSeconds;
	itspec.it_value.tv_nsec = durationNanoseconds;
	itspec.it_interval.tv_sec = durationSeconds;
	itspec.it_interval.tv_nsec = durationNanoseconds;

	if ( timer_settime((MytimerId), 0, &itspec, NULL) == -1)
	{
		return -1;
	}

	printf("Timer created\n");
	return 0;
}

void MyTimer::AddEntitiesToBeNotified(EventList* event)
{
	printf("MyTimer::AddEntitiesToBeNotified\n");
	//NotifyObjects.push_back(event);
	NotifyObjects = event; //should be a list
	NotifyObjects->DisplayEventsInList();

}
