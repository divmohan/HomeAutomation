/*
 * testq1.cc
 *
 *  Created on: 26-Oct-2017
 *      Author: div
 */
/*
 * TestMsgQueueProc1.cc
 *
 *  Created on: 25-Oct-2017
 *      Author: div
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <stdint.h>


using namespace std;

mqd_t recvId, sendId;

typedef struct
{
	int id;
	char name[10];
}MessageFormat;

int main()
{
	struct mq_attr queueAttributes;

	queueAttributes.mq_flags   = 0;
	queueAttributes.mq_maxmsg  = 10;  //Change limit later if required
	queueAttributes.mq_msgsize = sizeof(MessageFormat); //MAX_MESSAGE_SIZE;
	queueAttributes.mq_curmsgs = 0;

	MessageFormat data;
	memset(&data, 0, sizeof(data));
	data.id = 11;
	memcpy(data.name,"Test1",sizeof("Test1"));

	const char* queueName = "/test1";  //GetQueueName(processId);
	cout<<"QueueName"<<queueName<<endl;

	/* create/open the process message queue for reading*/
	recvId = mq_open(queueName, O_CREAT | O_RDONLY ,  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, &queueAttributes);
	if ( -1 == recvId )
	{
		cout<<"[Test1]:mq_open() failed:\n"<<strerror(errno)<<endl;

	}

	const char* sendQueueName = "/test2";  //GetQueueName(processId);
	cout<<"QueueName"<<sendQueueName<<endl;
	sendId = mq_open(sendQueueName, O_WRONLY| O_NONBLOCK, 0644, &queueAttributes);
	if ( -1 == sendId )
	{
		cout<<"[Test1]:mq_open() failed:\n"<<strerror(errno)<<endl;

	}

	int returnCode = mq_send(sendId, (char*)&data, sizeof(data), 0);
	if ( returnCode < 0)
	{
		cout<<"[Test1]Send Error:"<<strerror(errno)<<"Qid"<<int(sendId)<<endl;
	}

	MessageFormat recvData;
	memset(&recvData, 0, sizeof(recvData));

    returnCode = mq_receive(recvId, (char*)&recvData, 1024, NULL);
	if ( 0 > returnCode )
	{
		cout<<"[Test1] Receive Error:"<<strerror(errno)<<endl;

	}

	printf(" Received Message: Source ID: %d, Process Name:%s\n", int(recvData.id), recvData.name);

	//  char data[] = "Process Sending data";
	//msgObj.BroadcastMessage(&data, sizeof(data), uint8_t( 2));

	while(1)
	{
		sleep(1);
	}


}




