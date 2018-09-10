/*
 * NotificationApp.h
 *
 *  Created on: 17-Oct-2017
 *      Author: div
 */

#ifndef INC_NOTIFICATIONAPP_H_
#define INC_NOTIFICATIONAPP_H_

#include "../../../CommonIncludes/BaseApp.h"

#define INPUTPORT 50012
#define MAX_INPUT_LENGTH 100

#include "MyTimer.h"
#include "EventList.h"

class NotificationApp: public BaseApp
{
	int socketFd;
	int serverSocketFd;


public:

	MyTimer timerObj;
	EventList eventObj;


	int SendResponse(const MessageType msgType, const ProcessId destId, const uint8_t msgId);

	NotificationApp();
	~NotificationApp();

	int GetSocketFd();

	EventList* GetEventListObject();
	MyTimer* GetTimerObject();

	int InitializeModule();
	int InitializeSocketConnection();
	void ProcessMessage( MessageFormat* receivedMsg );
};



#endif /* INC_NOTIFICATIONAPP_H_ */
