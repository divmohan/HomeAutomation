/**
 * @file
 * Defines the functions relating to event class
 */

#include <stdio.h>
#include <string.h>  //for memset
#include <map>
#include "Event.h"


int GetMonthFromMonthName(std::string month)
{
	std::map<std::string,int> calendarMonths
	    {
	        { "jan", 0 },
	        { "feb", 1 },
	        { "mar", 2 },
	        { "apr", 3 },
	        { "may", 4 },
	        { "jun", 5 },
	        { "jul", 6 },
	        { "aug", 7 },
	        { "sep", 8 },
	        { "oct", 9 },
	        { "nov", 10 },
	        { "dec", 11 }
	    };

	    const auto iter = calendarMonths.find( month );

	    if( iter != calendarMonths.end() )
	    	return (iter->second);
	    else
	    	return -1;
}

long GetCurrentYear()
{
	time_t currentTime;
	struct tm *localTime;

	time( &currentTime );                   // Get the current time
	localTime = localtime( &currentTime );

	return (localTime->tm_year + 1900);
}

Event::Event()
{
	eventText = " ";
	notificationType = NotificationType::OneTime;
	memset(&notificationTimestamp, 0, sizeof(notificationTimestamp));

	eventNotified = false;
}

void Event::SetNotificationYear( const int year )
{
	notificationTimestamp.tm_year = year ;
}

/*! Sets the time at which the event should be notified
*/
void Event::SetNotificationTime(const uint8_t day,std::string month,const long year,const uint8_t minute, const uint8_t hour)
{
	//printf("SetNotificationTime()\n");

	notificationTimestamp.tm_mday = day;

	int monthNumber = GetMonthFromMonthName(month);
	if (-1 == monthNumber)
	{
		return;
	}
	notificationTimestamp.tm_mon  = monthNumber;

	if ( 0 == year)
	{
		//get the current year
		notificationTimestamp.tm_year = GetCurrentYear();
	}
	else
	{
		notificationTimestamp.tm_year = int (year) ;
	}

	if ( ( 0 == hour) && (0 == minute ) ) //!TODO:00:00 wont work. should we keep default as 00:00?
	{
		//Assign default time
		notificationTimestamp.tm_min  = DEFAULT_NOTIFY_TIME_MINUTE;
		notificationTimestamp.tm_hour = DEFAULT_NOTIFY_TIME_HOUR;
	}
	else
	{
		notificationTimestamp.tm_min  = minute;
		notificationTimestamp.tm_hour = hour;
	}

	//printf("\nNotification Time:day: %d month:%d year:%d\n",notificationTimestamp.tm_mday,notificationTimestamp.tm_mon,notificationTimestamp.tm_year);
}


void Event::SetNotificationType( const std::string type)
{

	//!TODO: Direct comparison didn work. figure out why or keep using strcmp.
   if (0 == strcmp(type.c_str(),"yearly"))    //See if you have to use string.compare
   {
	   notificationType = NotificationType::Yearly;
   }
   else if (0 == strcmp(type.c_str(),"monthly")) //( type == "monthly")
   {
	   notificationType = NotificationType::Monthly;

   }
   else if (0 == strcmp(type.c_str(),"daily")) //( type == "monthly")
   {
	   notificationType = NotificationType::Daily;

   }
   else
   {
	   //Default is one time
	   notificationType = NotificationType::OneTime;
   }

	printf("Notification Type: %d\n",notificationType);
}

void Event::SetEventText(const std::string text)
{
	//eventText = text;
	eventText.assign(text);

	//printf("EventText: %s\n",eventText.c_str());
}


std::string& Event::GetEventText()
{
	return eventText;
}

struct tm& Event::GetNotificationTime()
{
	//printf("Notification Time:day: %d year:%d\n",notificationTimestamp.tm_mday,notificationTimestamp.tm_year);
  printf("EVENT(): Hour:%d Min:%d,day:%d year:%d\n", notificationTimestamp.tm_hour,notificationTimestamp.tm_min,notificationTimestamp.tm_mday, notificationTimestamp.tm_year);
  return notificationTimestamp;
}

NotificationType Event::GetNotificationType()
{
  return notificationType;
}

bool Event::GetEventNotified()
{
  return eventNotified;
}

void Event::SetEventNotified( const bool notified )
{
	eventNotified = notified;
}

void Event::SetEventIdentifier( const std::string msg_identifier )
{
   eventId.assign(msg_identifier);;
}


std::string& Event::GetEventIdentifier()
{
   return eventId;
}
