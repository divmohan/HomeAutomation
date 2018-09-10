/*
 * BaseApp.h
 *
 *  Created on: 13-Oct-2017
 *      Author: div
 */

#ifndef INC_BASEAPP_H_
#define INC_BASEAPP_H_
#include <string>
#include "../../../CommonIncludes/CommonDefinitions.h"
#include "../../../CommonIncludes/MessageExchange.h"

typedef struct
{
   ProcessId procId;
   const char* procName;  //!TODO:Should we make it string?
   bool procAlive;
   std::string portNumber;
}ProcessDetails;


class BaseApp
{
   ProcessId processUniqueId;
   uint8_t   processCount;

   MessageExchange* queueHandle;

   int GetProcessIndex();

public:
    BaseApp( ProcessId processid);
    ~BaseApp();
    int  InitializeCommunicationChannel();
	int  SendMessage(const MessageFormat* sendData );
	int  ReceiveMessage(MessageFormat* receiveBuffer);
	void UninitializeCommunicationChannel();

	ProcessId GetProcessId(const int processIndex );
	const char* GetProcessName( const int processIndex );
};


#endif /* INC_BASEAPP_H_ */
