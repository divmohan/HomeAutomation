/*
 * MonitoringApp.h
 *
 *  Created on: 24-Sep-2017
 *      Author: div
 */

#ifndef INC_MONITORINGAPP_H_
#define INC_MONITORINGAPP_H_

#include<stdint.h>
#include <mqueue.h>
#include <string>
#include "../../../CommonIncludes/BaseApp.h"
#include "../../../CommonIncludes/MessageTransfer.h"

#define internalQueue "/MonAppIntQ1"

#define MAX_MSGS_WITHOUT_RESPONSE 3

class MonitoringApp : public BaseApp
{
	//Inter thread queue
	mqd_t internalQueueId;
	bool systemUp;
	bool reportFault;
	uint8_t retryCount;

	uint8_t processAckStatus;
    uint8_t expectedAckStatus;


    void RaiseSystemFault();
	int InitializeInternalCommunication();
	int CloseInternalCommunication();

	public:
    MonitoringApp();
	int InitializeModule();
	int StartProcesses();

	int InitiateHandshake();
	int UpdateSystemInitStatus();

	int SendMessageToAllProcesses(const MessageType msgType, const uint8_t dataLength, const uint8_t* data);

	int SendMonitoringMessage();
    int ReadAndProcessInternalMessage();
	void CheckProcessStatus();

	int NotifyProcessFault();
	int InitiateSystemRestart();

	int ReadFromInternalQueue(MessageFormat *receivedData);
	int WriteToInternalQueue(const MessageFormat *sendData);

	void ProcessMessage(MessageFormat* receiveMsg);
	//Add synchronization here. Sync with cloud on receiving state change message
};



#endif /* INC_MONITORINGAPP_H_ */
