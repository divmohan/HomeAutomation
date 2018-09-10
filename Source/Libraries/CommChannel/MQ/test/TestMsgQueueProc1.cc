/*
 * TestMsgQueueProc1.cc
 *
 *  Created on: 25-Oct-2017
 *      Author: div
 */


#include <stdio.h>
#include <string.h>
#include "../../../../CommonIncludes/MessageExchange.h"

int main()
{
  MessageExchange msgObj;
  int returnCode = msgObj.InitializeBroadcastSession(ProcessId(0), 2);

  MessageFormat data;
  memset(&data, 0, sizeof(data));
  data.destId = ProcessId::BroadcastId;
  data.sourceId = ProcessId (1);

//  char data[] = "Process Sending data";
  msgObj.BroadcastMessage(&data, sizeof(data), uint8_t( 2));

  MessageFormat recvData;
  memset(&recvData, 0, sizeof(recvData));
  msgObj.ReceiveMessage(&recvData, sizeof(recvData));

  printf(" Received Message: Source ID: %d\n", int(recvData.sourceId));
}
