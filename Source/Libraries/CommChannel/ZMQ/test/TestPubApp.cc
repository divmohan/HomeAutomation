/*
 * testPubSub.cpp
 *
 *  Created on: 14-Sep-2017
 *      Author: div
 */

#include <unistd.h>
#include "MessageTransfer.h"

int main()
{
    MessageTransfer commExchange(CommRole::Publisher);
    std::string portNumber("5603");
    commExchange.InitializeSession(portNumber, "127.0.0.1"); //!TODO: Check why default arg is not working

    //std::string data("Publishing data for test");

    uint8_t sendData = 0;

    for (int ii=0;ii<10;ii++ )
    {
    	sleep(1);
    	//data.append(ii);
    	commExchange.SendData(&sendData,size_t(sendData));
    	sendData++;
    }

    commExchange.TerminateSession();
}
