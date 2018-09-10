/**
 * @file
 * Defines the event class for holding the event details
 */

#ifndef INC_NOTIFICATIONEVENTS_H_
#define INC_NOTIFICATIONEVENTS_H_

#include <string>
#include <vector>
#include <time.h>
#include "NotificationCommon.h"

#define DEFAULT_NOTIFY_TIME_MINUTE 45
#define DEFAULT_NOTIFY_TIME_HOUR   22

class Event
{
	std::string eventText;

	std::string eventId;

	struct tm notificationTimestamp;
    //whether one time or periodic
    NotificationType notificationType;

    bool eventNotified;

public:

    Event();

    std::string& GetEventText();
    std::string& GetEventIdentifier();
    struct tm& GetNotificationTime();
    NotificationType GetNotificationType();
    bool GetEventNotified();

    void SetEventIdentifier( const std::string msg_identifier );
    void SetNotificationYear( const int year );
    void SetEventText(const std::string text);
    void SetNotificationTime(const uint8_t day,std::string month,const long year = 0,const uint8_t minute = 0, const uint8_t hour = 0);
    void SetNotificationType( const std::string type);
    void SetEventNotified( const bool notified );

    //Sync with google calendar API.
};

//Generic functions
long GetCurrentYear();
int GetMonthFromMonthName(std::string month);


#endif /* INC_NOTIFICATIONEVENTS_H_ */
