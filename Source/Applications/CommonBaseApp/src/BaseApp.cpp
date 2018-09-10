/*
 * BaseApp.cpp
 *
 *  Created on: 13-Oct-2017
 *      Author: div
 */

#include <iostream>
#include <string.h>
#include "BaseApp.h"

using namespace std;

ProcessDetails ProcessList [] = { ProcessId::MonitoringAppId,  "MonitoringApp",   false, "9016",
		ProcessId::NotificationAppId, "./NotificationApp", false, "9017"
};

BaseApp::BaseApp(ProcessId processid)
{
	processUniqueId = processid;
	queueHandle = NULL;

	processCount = (sizeof (ProcessList))/ (sizeof(ProcessDetails));

	cout<<"Debug: Process Count"<<processCount;
}

int BaseApp::GetProcessIndex()
{
	for (int ii=0; ii<processCount; ii++)
	{
		if ( processUniqueId == ProcessList[ii].procId )
		{
			return ii;
		}
	}
	return ERROR_VALUE;
}

ProcessId BaseApp::GetProcessId(const int processIndex )
{
	if ( processIndex < 0 || processIndex > (processCount - 1) )
	{
		return ProcessId::NotDefined;
	}
	else
	{
		cout<<"Process Id"<<uint8_t(ProcessList[processIndex].procId)<<endl;
		return ProcessList[processIndex].procId ;

	}
}

const char* BaseApp::GetProcessName( const int processIndex )
{
	if ( processIndex < 0 || processIndex > (processCount - 1) )
	{
		return NULL;
	}
	else
	{
		cout<<"Process Name"<<ProcessList[processIndex].procName<<endl;
		return ProcessList[processIndex].procName ;

	}
}



int  BaseApp::InitializeCommunicationChannel()
{
	int returnCode = ERROR_VALUE;

	int processIndex = GetProcessIndex();
	if ( -1 == processIndex)
		return returnCode;

	queueHandle = new MessageExchange();
	returnCode = queueHandle->InitializeBroadcastSession(processUniqueId,processCount);

	return returnCode;
}


int  BaseApp::SendMessage(const MessageFormat* sendData )
{
	size_t messageLength = 0;

	if ( sendData == NULL)
	{
		return ERROR_VALUE;
	}
	uint8_t sendBuffer[sizeof(MessageFormat)];
	memset(sendBuffer, 0,sizeof(sendBuffer));

	messageLength = sizeof(sendData->msgHeader);
	memcpy(sendBuffer,&(sendData->msgHeader),messageLength);

	memcpy(&sendBuffer[messageLength],sendData->msgData,sendData->msgHeader.dataLength);
	messageLength = messageLength + sendData->msgHeader.dataLength;

	cout<<"[BaseApp]Send Data: MessageLength :"<<messageLength<<endl;
	for(int ii=0; ii<messageLength; ii++)
	{
		printf("%x ", sendBuffer[ii]);
	}

	int returnCode = ERROR_VALUE;
	returnCode = queueHandle->BroadcastMessage(sendBuffer,messageLength, processCount);
	return returnCode;
}


int BaseApp::ReceiveMessage(MessageFormat* receiveBuffer )
{
	if ( NULL == receiveBuffer )
		return ERROR_VALUE;

	int returnCode = ERROR_VALUE;

	size_t dataLength = sizeof(MessageFormat);
	uint8_t recvBuffer[dataLength];
	memset(recvBuffer,0,sizeof(recvBuffer));

	returnCode = queueHandle->ReadMessage(recvBuffer,dataLength);
	//cout<<"BaseApp::ReceiveMessage():returnCode:"<<returnCode<<endl;

	for (int ii=0;ii<dataLength;ii++)
	{
		//cout<<recvBuffer[ii]<<endl;
	}

	//Copy to recv message structure
	uint8_t headerLength = sizeof(receiveBuffer->msgHeader);

	memcpy(&receiveBuffer->msgHeader,recvBuffer, headerLength );
	memcpy(receiveBuffer->msgData, &recvBuffer[headerLength], receiveBuffer->msgHeader.dataLength);

	return returnCode;
}

BaseApp::~BaseApp()
{
	int returnCode = queueHandle->TerminateBroadcastSession(processUniqueId, processCount);

}
