/*
 * MsgExchangeApp.cpp
 *
 *  Created on: 10-Sep-2017
 *      Author: div
 */

#include <errno.h>
#include <iostream>
#include <string.h>

#include "../../../../CommonIncludes/MessageTransfer.h"


MessageTransfer::MessageTransfer( CommRole transferRole )
{
	commProtocol.append("tcp");
	role = transferRole;
	commContext = NULL;
}

int MessageTransfer::InitializeSession(const string portNumber, const string ipAddress )
{
	commContext = zmq_ctx_new();

	int returnCode = ERROR_VALUE;
	if ( CommRole::Publisher == role )
	{

		std::string bindAddress = commProtocol + "://*:" + portNumber;
		dataSocket = zmq_socket(commContext, ZMQ_PUB);
		returnCode = zmq_bind(dataSocket,bindAddress.c_str());
		if ( ERROR_VALUE == returnCode)
		{
			cout<<"ZMQ::BIND() error: %s\n"<<strerror(errno);
			return ERROR_VALUE;
		}
	}
	else
	{
		//Validate IP an port NUmber

		std::string connectAddress = commProtocol + "://" + ipAddress + ":" + portNumber ;

		//debug
		cout<<"Connect Address"<<connectAddress;

		dataSocket = zmq_socket(commContext, ZMQ_SUB);
		returnCode = zmq_connect(dataSocket,connectAddress.c_str());
		if ( ERROR_VALUE == returnCode)
		{
			cout<<"ZMQ::CONNECT() error: %s\n"<<strerror(errno);
			return ERROR_VALUE;
		}
	}

	return SUCCESS;
}


int MessageTransfer::SubcribeToAllMessages()
{
	int returnCode = zmq_setsockopt( dataSocket, ZMQ_SUBSCRIBE,"", 0);
	if ( ERROR_VALUE == returnCode )
	{
		cout <<"ZMQ::SetSocketOption() error:%s\n"<<strerror(errno);
	}

	return returnCode;
}

int MessageTransfer::SetSocketOption(const int socketOption, const void* optionValue, size_t optionLength)
{

	int returnCode = ERROR_VALUE;

	returnCode = zmq_setsockopt( dataSocket, socketOption,optionValue, optionLength);
	if ( ERROR_VALUE == returnCode )
	{
		cout <<"ZMQ::SetSocketOption() error:%s\n"<<strerror(errno);
	}
	return SUCCESS;
}

int MessageTransfer::SendData(const void* data, const size_t dataLength )
{
	int returnCode = ERROR_VALUE;

	if ( ( NULL == data ) || ( 0 == dataLength) )
		return ERROR_VALUE;

	cout<<"ZMQ Sending data"<<data<<endl;

	//returnCode = zmq_send(dataSocket, zmq_msg_t *msg, int flags); //USe zmq_msg_t if sending same data many times?
	returnCode = zmq_send(dataSocket, data, dataLength, ZMQ_NOBLOCK);
	if ( ERROR_VALUE == returnCode )
	{
		cout<<"ZMQ::SEND() error:%s"<<strerror(errno);
		return returnCode; //else print the bytes sent??
	}

	return SUCCESS;
}


int MessageTransfer::ReceiveData(void* data, const size_t dataLength)
{
	int returnCode = ERROR_VALUE;

	if ( ( NULL == data ) || ( 0 == dataLength) )
		return ERROR_VALUE;

	//returnCode = zmq_send(dataSocket, zmq_msg_t *msg, int flags); //USe zmq_msg_t if sending same data many times?
	returnCode = zmq_recv(dataSocket, data, dataLength, ZMQ_NOBLOCK ); //Blocking receive
	if ( ERROR_VALUE == returnCode )
	{
		//cout<<"ZMQ::RECV() error:%s"<<strerror(errno);
		return returnCode; //else print the bytes sent??
	}

	return SUCCESS;
}

void MessageTransfer::TerminateSession()
{
	//Close sockets
	zmq_close(dataSocket);

	//destroy context
	zmq_ctx_term(commContext);
}
