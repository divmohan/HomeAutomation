/*
 * CommChannelApis.cpp
 *
 *  Created on: 20-Oct-2017
 *      Author: div
 */

#include <string.h>
#include <iostream>

#include "../../../../CommonIncludes/MessageExchange.h"
#include "../../../../CommonIncludes/CommonDefinitions.h"

using namespace std;

const char* ProcessQueue[] = { "/MonitorPQ",
		                       "/NotifyPQ"
		                     };


const char* GetQueueName(ProcessId processId)
{
	return ProcessQueue[uint8_t(processId)];
}

MessageExchange::MessageExchange()
{
	broadcastMessageIds.receiveQId = mqd_t(ERROR_VALUE);
	for (int ii=0; ii<(MAX_PROCESS_COUNT -1 ); ii++)
	{
		broadcastMessageIds.sendQIds[ii] = mqd_t (ERROR_VALUE);
	}

	memset(&queueAttributes, 0, sizeof(queueAttributes));
}

int MessageExchange::InitializeBroadcastSession(const ProcessId processId, const uint8_t processCount)
{
	cout<<"THIS IS THE LATEST ONE"<<endl;

	queueAttributes.mq_flags   = 0;
	queueAttributes.mq_maxmsg  = 10;  //Change limit later if required
	queueAttributes.mq_msgsize = sizeof(MessageFormat); //MAX_MESSAGE_SIZE;
	queueAttributes.mq_curmsgs = 0;

	const char* recvQueueName = GetQueueName(processId);

	/* create/open the process message queue for reading*/
	broadcastMessageIds.receiveQId = mq_open(recvQueueName, O_CREAT | O_RDONLY ,  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, &queueAttributes);
	if ( ERROR_VALUE == broadcastMessageIds.receiveQId )
	{
		cout<<"MessageExchange::InitializeBroadcastSession():mq_open() failed:\n"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}

	cout<<"Open own queue for receiving: "<<recvQueueName<<"Queue ID"<<broadcastMessageIds.receiveQId<<endl;

	//Create/open other queues in send mode

	for (uint8_t ii=0,jj = 0; ii<processCount; ii++)
	{
		if ( ii != (uint8_t)processId)
		{
			const char* sendQueueName = GetQueueName(ProcessId(ii));
			broadcastMessageIds.sendQIds[jj] = mq_open(sendQueueName, O_CREAT | O_WRONLY| O_NONBLOCK, 0644, &queueAttributes);
			if ( ERROR_VALUE == broadcastMessageIds.sendQIds[jj] )
			{
				cout<<"MessageExchange::InitializeBroadcastSession():mq_open() failed:\n"<<strerror(errno)<<endl;
				return ERROR_VALUE;
			}

			cout<<"MessageExchange::InitializeBroadcastSession():mq_open() success:\n"<<int(jj)<<int(broadcastMessageIds.sendQIds[jj])<<endl;
			jj++;
		}

		cout<<"Process Id:"<<int(processId)<<"ProcessCount"<<int(processCount)<<endl;
	}

	return SUCCESS;
}

int MessageExchange::BroadcastMessage(const void* data, const size_t dataLength, const uint8_t processCount)
{
	int returnCode = ERROR_VALUE;

	for (int ii = 0; ii< (processCount -1); ii++)
	{
		returnCode = mq_send(broadcastMessageIds.sendQIds[ii], (char*)data, dataLength, 0);
		if ( returnCode < 0)
		{
			cout<<"MessageExchange::BroadcastMessage:Send Error:"<<strerror(errno)<<"Qid"<<int(broadcastMessageIds.sendQIds[ii])<<"DataLength"<<dataLength<<endl;
			return ERROR_VALUE;
		}

		cout<<"MessageExchange::BroadcastMessage:message send to"<<int(broadcastMessageIds.sendQIds[ii])<<"successfully"<<endl;

	}

	MessageFormat sentData;
	memset(&sentData,0,sizeof(sentData));
	memcpy(&sentData, data, sizeof(sentData));

	printf("Sent Data dest Id:%x",(sentData.msgHeader.destId));
	//cout<<"Data sent"<<endl;
	for (int ii=0; ii<dataLength; ii++)
	{
		//cout<<" "<<*(data + ii)<<endl;
	}
	return SUCCESS;
}


int MessageExchange::ReadMessage(void* data, size_t dataLength)
{
	int returnCode = mq_receive(broadcastMessageIds.receiveQId, (char*)data, MAX_MESSAGE_SIZE, NULL);
	if ( 0 > returnCode )
	{
		cout<<"MessageExchange::ReceiveMessage: Receive Error:"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}
	cout<<"MessageExchange::ReceiveMessage: Received Successfully:Bytes"<<returnCode<<endl;

	return SUCCESS;
}



int MessageExchange::TerminateBroadcastSession(const ProcessId processId, const uint8_t processCount)
{
	//Close queues

	if ( (mqd_t)-1 == mq_close(broadcastMessageIds.receiveQId))
	{
		cout<<"MessageExchange::TerminateBroadcastSession(): Close error"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}

	for (int ii=0; ii<(processCount -1); ii++ )
	{
		if ( (mqd_t)-1 == mq_close(broadcastMessageIds.sendQIds[ii]))
		{
			cout<<"MessageExchange::TerminateBroadcastSession(): Close error"<<strerror(errno)<<endl;
			return ERROR_VALUE;
		}
	}

	if ( (mqd_t)-1 == mq_unlink(GetQueueName(processId)))
	{
		cout<<"MessageExchange::TerminateBroadcastSession(): Queue unlink error"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}

	return SUCCESS;
}



