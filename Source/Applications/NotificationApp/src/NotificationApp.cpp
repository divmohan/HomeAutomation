/*
 * NotificationApp.cpp
 *
 *  Created on: 17-Oct-2017
 *      Author: div
 */

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <pthread.h>

#include <Python.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <sstream>
#include <iterator>
#include <string>


#include "../../../CommonIncludes/CommonDefinitions.h"
#include "NotificationApp.h"


using namespace std;

NotificationApp::NotificationApp():BaseApp(ProcessId::NotificationAppId)
{
	socketFd = -1 ;
	serverSocketFd = -1;
}

NotificationApp::~NotificationApp()
{
	close(socketFd);
	close(serverSocketFd);
}

int NotificationApp::InitializeModule()
{
	int returnCode = ERROR_VALUE;

	returnCode = InitializeCommunicationChannel();

	return returnCode;
}

int NotificationApp::SendResponse(const MessageType msgType, const ProcessId destId, const uint8_t msgId)
{
	MessageFormat sendMessage;
	memset(&sendMessage, 0, sizeof(sendMessage));

	sendMessage.msgHeader.destId     = destId;
	sendMessage.msgHeader.sourceId   = ProcessId::NotificationAppId;
	sendMessage.msgHeader.msgType    = msgType;
	sendMessage.msgHeader.msgId      = msgId;

	int returnCode = SendMessage(&sendMessage);
	if ( SUCCESS != returnCode)
	{
		cout<<"[NotificationApp] Error sending response message"<<endl;
		return ERROR_VALUE;
	}

	return SUCCESS;
}

void NotificationApp::ProcessMessage( MessageFormat* receivedMsg )
{
	cout<<"[NotificationApp] ProcessMessage"<<endl;
	switch( receivedMsg->msgHeader.msgType )
	{
	case  MessageType::Handshake:
	case MessageType::Heartbeat:
	{
		cout<<"[NotificationApp] Message received "<<uint8_t(receivedMsg->msgHeader.msgType)<<endl;

		//send ack
		int returnCode = SendResponse(MessageType::HeartbeatAck, receivedMsg->msgHeader.sourceId, receivedMsg->msgHeader.msgId);
		break;
	}

	default:
		break;
	}
}

int NotificationApp::InitializeSocketConnection()
{
	socklen_t clientLength;

	struct sockaddr_in serverAddress, clientAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));

	int returnCode = ERROR_VALUE;

	serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocketFd < 0)
	{
		cout<<"[NotificationApp] error opening socket"<<endl;
		return ERROR_VALUE;
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port        = htons(INPUTPORT);

	returnCode = bind(serverSocketFd, (struct sockaddr *) &serverAddress,sizeof(serverAddress));

	if (returnCode < 0)
	{
		printf("[NotificationApp]:ERROR on binding:%s\n", strerror(errno));
		return ERROR_VALUE;
	}

	listen(serverSocketFd,1);

	clientLength = sizeof(clientAddress);
	socketFd     = accept(serverSocketFd,(struct sockaddr *) &clientAddress, &clientLength);
	if (socketFd < 0)
	{
		printf("[NotificationApp] ERROR on accept:%s\n", strerror(errno));
		return ERROR_VALUE;
	}

	return SUCCESS;
}


int NotificationApp::GetSocketFd()
{
	return socketFd;
}

//Helper function: to be replaced by wit.ai

void extractWordsFromString( string& text, vector<string>& tokenList)
{
	// Wrap text in stream.
	std::istringstream myStringStream{text};
	// Read tokens from stream into vector (split at whitespace).
	//	std::vector<std::string> words{std::istream_iterator<std::string>{myStringStream}, std::istream_iterator<std::string>{}};
	//tokenList{std::istream_iterator<std::string>{myStringStream}, std::istream_iterator<std::string>{} };


	std::copy(std::istream_iterator<std::string>(myStringStream),std::istream_iterator<std::string>(),back_inserter(tokenList));

}

MyTimer* NotificationApp::GetTimerObject()
{
   return (&timerObj);
}

EventList* NotificationApp::GetEventListObject()
{
   return (&eventObj);
}

void* InputHandling(void* notificationAppObj )
{
	cout<<"*************NotificationApp: ReminderHandling() Thread*************"<<endl;

	int returnCode = ERROR_VALUE;

	NotificationApp* notifyAppObj = static_cast<NotificationApp*>(notificationAppObj);

	returnCode = notifyAppObj->InitializeSocketConnection();
	if ( ERROR_VALUE == returnCode )
	{
		return (void *)0;
	}

	uint8_t recvBuffer[MAX_INPUT_LENGTH];
	while(1)
	{
		memset(recvBuffer,0, sizeof(recvBuffer));

		returnCode = read(notifyAppObj->GetSocketFd(),recvBuffer,MAX_INPUT_LENGTH);
		if ( returnCode < 0)
		{
			printf("[NotificationApp] Read error:%s\n",strerror(errno));
		}
		else if ( returnCode == 0)
		{
			//end of file
		}
		else
		{
			printf("[NotificationApp] Read data:%s\n",recvBuffer);

			vector <string> wordList = {};

			std::stringstream ss;
			for (size_t i = 0; i < returnCode; ++i)
			{
			    ss << recvBuffer[i];
			}
			std::string inputString = ss.str();

			extractWordsFromString(inputString, wordList);
			for(auto itr= wordList.begin();itr!=wordList.end(); itr++)
			{
				cout<<*itr<<endl;
			}

			//EventList *eventObj = (notifyAppObj->GetEventListObject());

			returnCode = (notifyAppObj->eventObj).ParseAndStoreReceivedEvent(wordList);

			(notifyAppObj->eventObj).DisplayEventsInList();

//			(notifyAppObj->GetTimerObject())->AddEntitiesToBeNotified(eventObj);

		}

		sleep(1);
	}
	return ((void*)0);
}

int main()
{
	pthread_t reminder_thread, inputHandling_thread;

	NotificationApp notifyAppObj;
	int returnCode = ERROR_VALUE;

	//Initialize the message communication
	returnCode = notifyAppObj.InitializeModule();

	(notifyAppObj.timerObj).CreateTimer(10,0);
	(notifyAppObj.timerObj).AddEntitiesToBeNotified(&(notifyAppObj.eventObj));

	//Start monitoring the processes
	if( 0 != pthread_create(&inputHandling_thread, NULL, &InputHandling, &notifyAppObj) )
	{
		cout<<"[NotificationApp]:Error creating thread"<<endl;
	}



	MessageFormat receiveMsg;
	memset(&receiveMsg, 0, sizeof(receiveMsg));
	while (1)
	{
		//receive messages
		returnCode = notifyAppObj.ReceiveMessage(&receiveMsg);
		if ( SUCCESS == returnCode)
		{
			cout<<"NotificationApp: Received Message successfully"<<endl;
			printf("ReceivedMessage:Type: %x, Source:%x\n",uint8_t(receiveMsg.msgHeader.msgType), uint8_t(receiveMsg.msgHeader.sourceId));

			notifyAppObj.ProcessMessage(&receiveMsg);
		}
		else
		{
			cout<<"NotificationApp: ReceiveMessageError"<<endl;
		}
		sleep(1);
	}

	if (0 != pthread_join(inputHandling_thread, NULL) )
	{
		cout<<"[NoticationApp] pthread_join() error"<<endl;
	}
}
