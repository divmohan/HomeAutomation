/**
 * @file
 * holds the different event lists and the functions to  handle data in these lists
 */

#ifndef INC_EVENTS_H_
#define INC_EVENTS_H_

#include <map>
#include "Event.h"

class EventList
{

	std::vector<Event*>DailyEventList;
	std::vector<Event*>MonthlyEventList;
	std::vector<Event*>YearlyEventList;
	std::vector<Event*>OneTimeEventList; //this list should be checked every time the timer expires

	std::map <NotificationType, std::vector<Event*>> EventMap;


public:

	EventList();
	~EventList();

	void InitializePythonModule( std::string event );

	void AddEventToList(Event* event);
	int RemoveEventFromList();

	void AddListsToMap();

	Event& GetEventFromList(NotificationType listKey);

	int AddNewEventType(); //!TODO: define later

	void UpdateYearlyEvents( struct tm* currentLocalTime );
    void UpdateOneTimeEvents( struct tm* currentLocalTime );
    void UpdateMonthlyEvents( struct tm* currentLocalTime );
    void UpdateDailyEvents( struct tm* currentLocalTime );
	void UpdateListOnTimerExpiry();

	int ParseAndStoreReceivedEvent(std::vector<std::string> &receivedTokens);

	//edit event details
	void DisplayEventsInList(); //debug
};



#endif /* INC_EVENTS_H_ */
