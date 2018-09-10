/*
 * TestSubApp.cpp
 *
 *  Created on: 14-Sep-2017
 *      Author: div
 */

#include <iostream>
#include <string.h>
#include "MessageTransfer.h"

int main()
{
	MessageTransfer commExchange(CommRole::Subscriber);
	std::string portNumber("5603");
	commExchange.InitializeSession(portNumber, "127.0.0.1"); //!TODO: Check why default arg is not working

	//Subsciber to something
	commExchange.SubcribeToAllMessages();

	char recvBuffer[20];

	while(1)
	{
		memset(recvBuffer, 0, sizeof(recvBuffer));
		int returnCode= commExchange.ReceiveData(recvBuffer,sizeof(recvBuffer));

		if (returnCode != -1)
		cout<<"Received Data"<<int(recvBuffer[0])<<endl;

	}

	commExchange.TerminateSession();
}


