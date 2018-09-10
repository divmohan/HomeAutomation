/*
 * CommonDefinitions.h
 *
 *  Created on: 24-Sep-2017
 *      Author: div
 */

#ifndef COMMONINCLUDES_COMMONDEFINITIONS_H_
#define COMMONINCLUDES_COMMONDEFINITIONS_H_

#include <stdint.h>

#define MAX_PROCESS_COUNT 2
#define ERROR_VALUE      -1
#define SUCCESS           0

enum class ProcessId: uint8_t   //Linked to queue name. watch when adding new processes
{
	BroadcastId        = 0xff,
	MonitoringAppId    = 0,
	NotificationAppId  = 1,
	HardwareAppId      = 2,
	UserInterfaceAppId = 3,
	NotDefined         = 10
};

enum class MessageType: uint8_t
{

	Heartbeat       = 1,
	Fault           = 2,   //System faults
	Status          = 3,
	Handshake       = 4,
	HeartbeatAck    = 5,
	NotDefined      = 0xff
};

#define MAX_MESSAGE_LENGTH     50

typedef struct
{
	ProcessId   sourceId;
	ProcessId   destId;
	MessageType msgType;
	uint8_t     msgId;  //How to generate?
	//Should we need priority?
	uint8_t     dataLength;

}MessageHeader;

//Message format between processes
typedef struct
{
	MessageHeader msgHeader;
	uint8_t     msgData[MAX_MESSAGE_LENGTH];

}MessageFormat;

#endif /* COMMONINCLUDES_COMMONDEFINITIONS_H_ */
