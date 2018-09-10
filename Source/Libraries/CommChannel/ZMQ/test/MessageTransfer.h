/*
 * MsgExchangeApis.h
 *
 *  Created on: 10-Sep-2017
 *      Author: div
 */

#ifndef INC_MSGTRANSFER_H_
#define INC_MSGTRANSFER_H_

#include <string>
#include "/usr/local/include/zmq.h"

#define ERROR_VALUE -1
#define SUCCESS      0

using namespace std;

enum class CommRole
{
	Publisher,
	Subscriber
};

class MessageTransfer
{
   //zmq::socket_t
   void *dataSocket;
   CommRole role;
   std::string commProtocol;

   void* commContext;
   //Add default socket option and arguments later

   public:
   MessageTransfer( CommRole role);
   int InitializeSession(const std::string portNumber, const std::string ipAddress = NULL);
   int SubcribeToAllMessages();
   int SetSocketOption(const int socketOption, const void* optionValue, size_t optionLength);
   int SendData(const void* data, const size_t dataLength );
   int ReceiveData( void* data, const size_t dataLength);
   void TerminateSession();
};
//Initialize pub-sub sockets

//Set Socket option

//Send message

//Receive message

//Unsubscribe

//Close sockets


#endif /* INC_MSGTRANSFER_H_ */
