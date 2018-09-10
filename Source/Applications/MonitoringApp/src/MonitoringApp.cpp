/*
 * MonitoringApp.cpp
 *
 *  Created on: 24-Sep-2017
 *      Author: div
 */

#include <unistd.h>
#include <iostream>
#include <error.h>
#include <string.h>
#include <pthread.h>
#include "../../../CommonIncludes/CommonDefinitions.h"
#include "MonitoringApp.h"



MonitoringApp::MonitoringApp():BaseApp(ProcessId::MonitoringAppId)
{
	// BaseApp(ProcessId::MonitoringAppId);
	systemUp        = false;
	retryCount      = 0;
	reportFault     = false;
	internalQueueId = ERROR_VALUE;

	expectedAckStatus = 0;
	processAckStatus  = 0; //each bit represents each process.1 = process responded, 0= no response;
}

int MonitoringApp::InitializeInternalCommunication()
{
	struct mq_attr queueAttributes;
	queueAttributes.mq_flags   = 0;
	queueAttributes.mq_maxmsg  = 10;  //Change limit later if required
	queueAttributes.mq_msgsize = sizeof(MessageFormat); //MAX_MESSAGE_SIZE;
	queueAttributes.mq_curmsgs = 0;
	internalQueueId = mq_open(internalQueue , O_CREAT | O_RDWR|O_NONBLOCK ,  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, &queueAttributes);
	if ( ERROR_VALUE == internalQueueId )
	{
		cout<<"MessageExchange::InitializeBroadcastSession():mq_open() failed:\n"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}
	return SUCCESS;
}

int MonitoringApp::WriteToInternalQueue(const MessageFormat *sendData)
{
	int returnCode = ERROR_VALUE;

	if ( NULL == sendData)
		return ERROR_VALUE;

	uint8_t sendBuffer[sizeof(sendData)];
	memset(sendBuffer, 0,sizeof(sendBuffer));

	uint8_t messageLength = sizeof(sendData->msgHeader);
	memcpy(sendBuffer,&(sendData->msgHeader),messageLength);

	memcpy(&sendBuffer[messageLength],sendData->msgData,sendData->msgHeader.dataLength);
	messageLength = messageLength + sendData->msgHeader.dataLength;

	cout<<"[MonitoringApp]Send Data: MessageLength :%x"<<messageLength<<endl;
	for(int ii=0; ii<messageLength; ii++)
	{
		printf("%x ", sendBuffer[ii]);
	}

	returnCode = mq_send(internalQueueId, (char*)sendBuffer, messageLength, 0);
	if ( 0 > returnCode )
	{
		cout<<"[MonitoringApp]::SendMessageInternalQueue: Error:"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}

	return SUCCESS;
}

int MonitoringApp::ReadFromInternalQueue( MessageFormat *receivedData)
{
	uint8_t recvBuffer[sizeof(receivedData)];
	memset(recvBuffer, 0, sizeof(recvBuffer));

	int returnCode = mq_receive(internalQueueId, (char*)recvBuffer, MAX_MESSAGE_SIZE, NULL);
	if ( 0 > returnCode )
	{
        returnCode = errno;
		return returnCode;
	}

	for (int ii=0;ii<sizeof(recvBuffer);ii++)
	{
		cout<<recvBuffer[ii]<<endl;
	}

	//Copy to recv message structure
	uint8_t headerLength = sizeof(receivedData->msgHeader);

	memcpy(&receivedData->msgHeader,recvBuffer, headerLength );
	memcpy(receivedData->msgData, &recvBuffer[headerLength], receivedData->msgHeader.dataLength);

	//cout<<"[MonitoringApp]::ReceiveMessage: Received Successfully:Bytes"<<returnCode<<endl;

	return SUCCESS;
}

int MonitoringApp::CloseInternalCommunication()
{
	if ( (mqd_t)-1 == mq_close(internalQueueId))
	{
		cout<<"MessageExchange::TerminateBroadcastSession(): Close error"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}

	if ( (mqd_t)-1 == mq_unlink(internalQueue))
	{
		cout<<"MessageExchange::TerminateBroadcastSession(): Queue unlink error"<<strerror(errno)<<endl;
		return ERROR_VALUE;
	}
	return SUCCESS;
}

int MonitoringApp::StartProcesses()
{
	cout<<"MonitoringApp::StartProcesses()"<<endl;

	for( int ii=1; ii< MAX_PROCESS_COUNT; ii++)
	{
		expectedAckStatus = expectedAckStatus | uint8_t(GetProcessId(ii));
		int childProcessId = fork();
		if ( 0 == childProcessId )
		{
			//in child processs start
			const char* childProcess = GetProcessName(ii);
			cout<<"MonitoringApp: child process: "<<childProcess<<endl;
			if ( NULL == childProcess)
			{
				cout<<"MonitoringApp:Error creating child process"<<endl;
				return ERROR_VALUE;
			}
			//			assert(NULL, childProcess); // !TODO: Check usage once
			int returnCode = execvp(childProcess, NULL);
			if ( 0 > returnCode)
			{
				cout<<"MonitoringApp: execvp() failed"<<strerror(errno)<<endl;
			}
		}
		else
		{
			cout<<"Parent Process"<<endl;
			return SUCCESS;
		}

	}
	return SUCCESS;
}

int MonitoringApp::InitializeModule()
{
	int returnCode = ERROR_VALUE;

	returnCode = InitializeCommunicationChannel();
	if ( ERROR_VALUE == returnCode )
	{
		cout<<"[MonitoringApp] Error initializing communication channels"<<endl;
		return ERROR_VALUE;
	}

	returnCode = InitializeInternalCommunication();
	if ( ERROR_VALUE == returnCode )
	{
		cout<<"[MonitoringApp] Error initializing internal communication channels"<<endl;
		return ERROR_VALUE;
	}
	return SUCCESS;
}

int MonitoringApp::SendMessageToAllProcesses(const MessageType msgType, const uint8_t dataLength, const uint8_t* data)
{
	MessageFormat sendMessage;
	memset(&sendMessage, 0, sizeof(sendMessage));

	sendMessage.msgHeader.destId     = ProcessId::BroadcastId;
	sendMessage.msgHeader.sourceId   = ProcessId::MonitoringAppId;
	sendMessage.msgHeader.msgType    = msgType;

	//Fill in msg id later

	sendMessage.msgHeader.dataLength = dataLength;
	memcpy(sendMessage.msgData, data, dataLength);

	int returnCode = SendMessage(&sendMessage);
	if ( SUCCESS != returnCode)
	{
		cout<<"[MonitoringApp] Error sending message"<<endl;
		return ERROR_VALUE;
	}

	return SUCCESS;
}

void MonitoringApp::RaiseSystemFault()
{
	MessageType type   = MessageType::Fault;
	uint8_t data       = processAckStatus; //!TODO:decode the process names from the value of processAckStatus at receiving end later
	uint8_t dataLength = sizeof(data);

	int returnCode = SendMessageToAllProcesses(type, dataLength, & data);
	if ( ERROR_VALUE == returnCode)
	{
		cout<<"[MonitoringApp]Failed to send system fault message"<<endl;
	}
}

void MonitoringApp::CheckProcessStatus()
{
	printf("[MonitoringApp]CheckProcessStatus() retryCount:%x\n",retryCount);
	printf("processAckStatus:%d ExpectedAckStatus:%d\n",processAckStatus,expectedAckStatus);

	if ( expectedAckStatus != processAckStatus)
	{
		if ( MAX_MSGS_WITHOUT_RESPONSE <= retryCount )
		{
			if (false == reportFault )
			{
				//Raise system fault with non responding process details
				RaiseSystemFault();
			}

			//Reset flag/counters
			processAckStatus = 0;
			retryCount = 0;
			reportFault = true;
		}
		else
		{
			//Keep resending
		}
	}
	else
	{
		cout<<"ExpectedAck matches with process ack"<<endl;
		//Received response from all processes.
		//System healthy
		processAckStatus = 0;
		retryCount       = 0;
		if (true == reportFault)
		{
			reportFault = false;
		}

		//if first time
		if ( false == systemUp)
		{
			systemUp = true;
		}
	}
}

int MonitoringApp::ReadAndProcessInternalMessage()
{
	MessageFormat receiveMessage;
	int returnCode = ERROR_VALUE;


	while (1)
	{
		memset(&receiveMessage, 0, sizeof(receiveMessage));
		returnCode = ReadFromInternalQueue(&receiveMessage);

		if ( EAGAIN == returnCode) //if no more message to read, return.
		{
			return SUCCESS;  //No more messages in queue
		}

		if ( MessageType::HeartbeatAck == receiveMessage.msgHeader.msgType)
		{
			printf("RECEIVED HEARTBEAT ACK FROM: %d\n ",receiveMessage.msgHeader.sourceId);
			//If received message handshake, update the process id and see if all processes responded
			processAckStatus = processAckStatus | uint8_t(receiveMessage.msgHeader.sourceId);
            printf("Updated processAckStatus:%x\n", processAckStatus);
		}
		else
		{
			//Do Nothing; Not expecting any other message currently.
		}

	}
}

int MonitoringApp::SendMonitoringMessage()
{
	MessageType type;
	if ( false == systemUp )
	{
		type    = MessageType::Handshake; //send handshake untill all processes are up
	}
	else
	{
		type    = MessageType::Heartbeat;
	}

	int returnCode = SendMessageToAllProcesses(type, 0, NULL );
	if ( SUCCESS != returnCode)
	{
		cout<<"[MonitoringApp]: SendMessage Failed\n"<<endl;
		return ERROR_VALUE;
	}
	else
	{
		//cout<<"[MonitoringApp]; Monitoring Message sent Successfully"<<endl;
		retryCount++ ;
	}

	return SUCCESS;
}


void* MonitorProcesses(void* monitoringApp )
{

	cout<<"*************MonitoringApp: MonitorProcesses() Thread*************"<<endl;

	int returnCode = ERROR_VALUE;

	MonitoringApp* monitorinAppObj = static_cast<MonitoringApp*>(monitoringApp);

	while(1)
	{
		returnCode = monitorinAppObj->SendMonitoringMessage();

		//read response from internal queue
		returnCode = monitorinAppObj->ReadAndProcessInternalMessage();

		monitorinAppObj->CheckProcessStatus();

		//sleep
		sleep(3);

	}

	return ((void*)0);
}

void MonitoringApp::ProcessMessage(MessageFormat* receiveMsg)
{

	if ( NULL == receiveMsg)
	{
		return;
	}

	int returnCode = ERROR_VALUE;
	switch( receiveMsg->msgHeader.msgType )
	{
	case MessageType::HeartbeatAck:
		//post it to monitoring thread
		returnCode = WriteToInternalQueue(receiveMsg);
		if (ERROR_VALUE == returnCode )
		{
			cout<<"[MonitoringApp] ProcessMessage():Error posting to internal queue"<<endl;
		}
		break;

	}
}

int main()
{

	pthread_t monitor_thread;

	MonitoringApp monitoringAppObj;

	//Start all the applications and wait for the system to be up
	monitoringAppObj.StartProcesses();


	//Initialize the message communication
	monitoringAppObj.InitializeModule();

	//Start monitoring the processes
	if( 0 == pthread_create(&monitor_thread, NULL, &MonitorProcesses, &monitoringAppObj) )
	{
		cout<<"[MonitoringApp]:Error creating thread"<<endl;
	}
	//do this later

	//monitoringAppObj.MonitorProcesses(); //do this later
	//startMonitoring(); //another thread.or all processes will send heartbeat message if not received rie fault
	//dont have to keep sending request.

	//receive messages
	int returnCode = ERROR_VALUE;
	MessageFormat receiveData;
	memset(&receiveData,0,sizeof(receiveData));

	while (1)
	{
		//ReceiveMessage();
		returnCode = monitoringAppObj.ReceiveMessage(&receiveData); //do this later
		if ( ERROR_VALUE == returnCode)
		{

			cout<<"[MonitoringApp] ReceiveMessage() error"<<endl;
		}
		else
		{
			//Process function
			monitoringAppObj.ProcessMessage(&receiveData);
		}
		sleep(1);
	}

	if (0 == pthread_join(monitor_thread, NULL) )
	{
		cout<<"[MonitoringApp] pthread_join() error"<<endl;
	}

	return 0;
}
