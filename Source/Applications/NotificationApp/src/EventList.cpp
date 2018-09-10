/**
 * @file
 * holds the different event lists and the functions to  handle data in these lists
 */

#include <numeric>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string.h>
#include <Python.h>

#include "NotificationCommon.h"
#include "EventList.h"

using namespace std;


EventList::EventList()
{
	//!TODO: Move it to initialize function?
	//Add the lists to map
	AddListsToMap();


	//!TODO: Should we just keep 2 lists? one time events and repeat events??

}

EventList::~EventList()
{
	Py_Finalize();
}

/*! Adds the event lists to the map with type of the event as the key.
 */
void EventList::AddListsToMap()
{
	EventMap[NotificationType::Daily]   = DailyEventList;
	EventMap[NotificationType::Monthly] = MonthlyEventList;
	EventMap[NotificationType::Yearly]  = YearlyEventList;
	EventMap[NotificationType::OneTime] = OneTimeEventList;
}

/*! Function parses the received input message, interprets the event details and stores the
 * details in the respective event lists
 *  Notificaition messages are expected in this format: <date> <text>
 *  Eg: 14 Feb Shyam's Birthday remind yearly
 */
int EventList::ParseAndStoreReceivedEvent(std::vector<std::string>& receivedTokens)
{
	//!TODO:Assign from a common pool
	Event*	eventObj = new Event();   //!TODO: Local pointer. wrong

	//Extract time //!TODO: Temporary method. will be replaced by wit.ai or the likes

	std::istringstream iss(receivedTokens[3]);
	std::string hour, minute;
	std::getline(iss, hour, ':');
	std::getline(iss, minute, ':');

	//std::cout << "Reminder time"<<hour <<minute<< std::endl;
	//Extract the msgId
	eventObj->SetEventIdentifier( receivedTokens[0] );


	eventObj->SetNotificationTime(std::stoi(receivedTokens[1]),receivedTokens[2], 0, std::stoi(minute), std::stoi(hour));

	const auto iter = std::find(receivedTokens.begin(),receivedTokens.end(),"remind");
	if( (iter + 1) != receivedTokens.end() )
	{
		int index = std::distance( receivedTokens.begin(), iter );
		//cout<<"[XXX]"<<receivedTokens[index]<<endl;
		index = index + 1;
		//cout<<"[XXX]"<<receivedTokens[index]<<endl;

		//set reminder type else type will be set to default
		eventObj->SetNotificationType(receivedTokens[index]);
	}

	//Use this if accumulate doesnt work.for (auto const& s : v) { result += s; }
	//std::string eventText;
	//eventText = accumulate(receivedTokens[2], receivedTokens[iter -1], eventText);

	int rangeEnd   =   std::distance( receivedTokens.begin(), iter ) ;
	int rangeStart =   4;
	const char* const delimiter = " ";
	std::ostringstream eventTextStream;
	std::copy(receivedTokens.begin()+ rangeStart, receivedTokens.begin()+rangeEnd,std::ostream_iterator<std::string>(eventTextStream, delimiter));

	//eventText = accumulate(receivedTokens.begin()+ rangeStart, receivedTokens.begin() + rangeEnd, eventText);


	//set event text
	eventObj->SetEventText(eventTextStream.str());

	//Add the event object to the event list
	AddEventToList(eventObj);

	//DisplayEventsInList();
	return 0;
}

/*! Function stores an event to a specific list based on event type
 */
void EventList::AddEventToList(Event* event)
{
	NotificationType notifyType = event->GetNotificationType();
	switch ( notifyType )
	{
	case NotificationType::Yearly:
		cout<<"Added events to yearly list"<<endl;
		YearlyEventList.push_back(event);
		break;
	case NotificationType::Monthly:
		cout<<"Added events to monthly list"<<endl;
		MonthlyEventList.push_back(event);
		break;
	case NotificationType::Daily:
		cout<<"Added events to daily list"<<endl;
		DailyEventList.push_back(event);
		break;
	case NotificationType::OneTime:
		cout<<"Added events to one time list"<<endl;
		OneTimeEventList.push_back(event);
		break;

	}
}


/*! Function displays the event details in a list. for debugging
 */
void EventList::DisplayEventsInList()
{
	if ( 0 == YearlyEventList.size() )
	{
		//cout<<"[XXX]List Empty"<<endl;
	}

	for (auto iter : YearlyEventList)
	{
		std::cout << (*iter).GetEventText() << "\n";
	}
}

void EventList::InitializePythonModule(std::string event)
{
	setenv("PYTHONPATH",".",1);

	//import module
	Py_Initialize();
	PyObject* moduleName = PyString_FromString((char*)"NotifyEvents");

	//PyObject* UIModuleString = PyString_FromString((char*)"NotificationInput");

	PyObject* UIModule       = PyImport_Import(moduleName);

	//reference to function
	if (UIModule == NULL)
	{
		cout<<"UIModule Error"<<endl;
		PyErr_Print();
		std::exit(1);

	}

	cout<<"Python module created"<<endl;

	PyObject* UIFunction     = PyObject_GetAttrString(UIModule,(char*)"sendMessage");
	PyObject* args           = PyTuple_Pack(1,PyString_FromString(event.c_str()));
	PyObject* myResult       = PyObject_CallObject(UIFunction, args);
	if (myResult != NULL )
	{
		printf("Python:Result of call: %d\n", PyList_Check(myResult));
	}
}

void EventList::UpdateYearlyEvents( struct tm* currentLocalTime )
{
	if ( NULL == currentLocalTime )
	{
		cout<<"Local time invalid"<<endl;
		return;
	}

	struct tm notificationTime;
	memset(&notificationTime,0,sizeof(notificationTime));




	for (auto iter : YearlyEventList)
	{
		//std::cout << (*iter).GetEventText() << "\n";
		memcpy(&notificationTime, &((*iter).GetNotificationTime()), sizeof(notificationTime));

		//Compare the timestamp
		if (    ( notificationTime.tm_mday == currentLocalTime->tm_mday )&&
				( notificationTime.tm_mon  == currentLocalTime->tm_mon )&&
				( notificationTime.tm_min  == currentLocalTime->tm_min )&&
				( notificationTime.tm_hour == currentLocalTime->tm_hour )
		)
		{
			if (false == (*iter).GetEventNotified() )
			{
				cout<<"EventNotification:"<<(*iter).GetEventText()<<"\n";
				(*iter).SetEventNotified(true);

				//!TODO: Set python path for this to work; else you get no module error

				std::stringstream outputEventStream;
				outputEventStream << (*iter).GetEventIdentifier() << " " << (*iter).GetEventText();

				std::string outputEvent = outputEventStream.str() ;
				InitializePythonModule( outputEvent );
			}
			else
			{
				cout<<"Event already notified. Do nothing"<<endl;
			}

		}
		else
		{
			//if year doesn't match and notified flag is set, reset it
			if (true == (*iter).GetEventNotified() )
			{//This is for future notifications
				(*iter).SetEventNotified(false);
			}
		}
	}


}

void EventList::UpdateOneTimeEvents( struct tm* currentLocalTime )
{
	if ( NULL == currentLocalTime )
	{
		cout<<"Local time invalid"<<endl;
		return;
	}
	struct tm notificationTime;
	memset(&notificationTime,0,sizeof(notificationTime));

	if ( false == OneTimeEventList.empty()) //if list not empty
	{

		for (auto iter : OneTimeEventList)
		{
			//std::cout << (*iter).GetEventText() << "\n";
			memcpy(&notificationTime, &((*iter).GetNotificationTime()), sizeof(notificationTime));

			//To be done
			if (    ( notificationTime.tm_mday == currentLocalTime->tm_mday )&&
					( notificationTime.tm_mon  == currentLocalTime->tm_mon )&&
					( notificationTime.tm_year == currentLocalTime->tm_year )&&
					( notificationTime.tm_min  == currentLocalTime->tm_min )&&
					( notificationTime.tm_hour == currentLocalTime->tm_hour )
			)
			{
				if (false == (*iter).GetEventNotified() )
				{
					cout<<"EventNotification:"<<(*iter).GetEventText()<<"\n";
					(*iter).SetEventNotified(true);

					std::stringstream outputEventStream;
					outputEventStream << (*iter).GetEventIdentifier() << " " << (*iter).GetEventText();

					std::string outputEvent = outputEventStream.str() ;
					InitializePythonModule( outputEvent );
					//Remove from list
					//!TODO:Not doing now.it should be removed only after the snooze option or ack

				}
				else
				{
					cout<<"Event already notified. This case should not arise"<<endl;
				}

			}
		}
	}

}

void EventList::UpdateMonthlyEvents( struct tm* currentLocalTime )
{
	if ( NULL == currentLocalTime )
	{
		cout<<"Local time invalid"<<endl;
		return;
	}
	struct tm notificationTime;
	memset(&notificationTime,0,sizeof(notificationTime));

	if ( false == MonthlyEventList.empty()) //if list not empty
	{

		for (auto iter : MonthlyEventList)
		{
			//std::cout << (*iter).GetEventText() << "\n";
			memcpy(&notificationTime, &((*iter).GetNotificationTime()), sizeof(notificationTime));

			if (    ( notificationTime.tm_mday == currentLocalTime->tm_mday )&&
					( notificationTime.tm_min  == currentLocalTime->tm_min )&&
					( notificationTime.tm_hour == currentLocalTime->tm_hour )
			)
			{
				if (false == (*iter).GetEventNotified() )
				{
					cout<<"EventNotification:"<<(*iter).GetEventText()<<"\n";
					(*iter).SetEventNotified(true);

					std::stringstream outputEventStream;
					outputEventStream << (*iter).GetEventIdentifier() << " " << (*iter).GetEventText();

					std::string outputEvent = outputEventStream.str() ;
					InitializePythonModule( outputEvent );

					//!TODO: wouldnt work if reminder needs to be set for end of month as months have diff no of days
					//Increment month
				}
				else
				{
					cout<<"Event already notified"<<endl;
				}

			}
		}
	}

}

void EventList::UpdateDailyEvents(struct tm* currentLocalTime )
{
	if ( NULL == currentLocalTime )
	{
		cout<<"Local time invalid"<<endl;
		return;
	}

	struct tm notificationTime;
	memset(&notificationTime,0,sizeof(notificationTime));


	for (auto iter : DailyEventList)
	{
		//std::cout << (*iter).GetEventText() << "\n";
		memcpy(&notificationTime, &((*iter).GetNotificationTime()), sizeof(notificationTime));

		//Compare the timestamp
		if (    ( notificationTime.tm_min  == currentLocalTime->tm_min )&&
				( notificationTime.tm_hour == currentLocalTime->tm_hour )
		)
		{
			if (false == (*iter).GetEventNotified() )
			{
				cout<<"EventNotification:"<<(*iter).GetEventText()<<"\n";
				(*iter).SetEventNotified(true);

				std::stringstream outputEventStream;
				outputEventStream << (*iter).GetEventIdentifier() << " " << (*iter).GetEventText();

				std::string outputEvent = outputEventStream.str() ;
				InitializePythonModule( outputEvent );
			}
			else
			{
				cout<<"Event already notified. Do nothing"<<endl;
			}

		}
		else
		{
			//if time doesnt match
			if (true == (*iter).GetEventNotified() )
			{//This is for future notifications
				(*iter).SetEventNotified(false);
			}
		}
	}


}

void EventList::UpdateListOnTimerExpiry()
{
	//!TODO:Post the timer expiry event and do it from another thread.
	//For annual events, compared the dates at a default time


	struct tm* currentLocalTime = NULL;
	std::chrono::time_point<std::chrono::system_clock> timeCheck;
	timeCheck = std::chrono::system_clock::now();
	std::time_t CurrentTime = std::chrono::system_clock::to_time_t(timeCheck);

	currentLocalTime = localtime(&CurrentTime);
	currentLocalTime->tm_year = currentLocalTime->tm_year + 1900; //tm_year gives the number of years from 1900

	//printf("UpdateListOnTimerExpiry(): So it does work!:%d\n", currentLocalTime->tm_year);

	//#if 0
	// YearlyEventList[0]->GetNotificationType();
	//If current time and date matches the default time of the event, notify
	//struct tm notificationTime = YearlyEventList[0]->GetNotificationTime();
	//printf("UpdateListOnTimerExpiry(): NotificationTime:%d\n", YearlyEventList[0]->GetNotificationTime().tm_hour);

	//cout<<"UpdatelistontimerExpiry()"<<endl;
	if ( false == YearlyEventList.empty()) //if list not empty
	{
		UpdateYearlyEvents(currentLocalTime);
	}


	UpdateOneTimeEvents( currentLocalTime );

	UpdateMonthlyEvents(currentLocalTime);

	if ( false == DailyEventList.empty()) //if list not empty
	{
		UpdateDailyEvents( currentLocalTime );
	}


}
