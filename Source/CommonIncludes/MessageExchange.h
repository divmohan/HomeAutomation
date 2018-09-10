/*
 * CommChannelApis.h
 *
 *  Created on: 20-Oct-2017
 *      Author: div
 */

#ifndef MQ_INC_MESSAGEEXCHANGE_H_
#define MQ_INC_MESSAGEEXCHANGE_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <stdint.h>
#include "CommonDefinitions.h"

#define MAX_MESSAGE_SIZE           1024

typedef struct
{
	mqd_t receiveQId;
	mqd_t sendQIds[MAX_PROCESS_COUNT - 1];
}MessageQueueIds;

class MessageExchange
{
	MessageQueueIds broadcastMessageIds;
	struct mq_attr queueAttributes;

public:
	MessageExchange();
    int InitializeBroadcastSession(const ProcessId processId, const uint8_t processCount);
    int BroadcastMessage(const void* data, const size_t dataLength, const uint8_t processCount);
    int ReadMessage( void* data, size_t dataLength);
    int TerminateBroadcastSession(const ProcessId processId, const uint8_t processCount);
};


#endif /* MQ_INC_MESSAGEEXCHANGE_H_ */
