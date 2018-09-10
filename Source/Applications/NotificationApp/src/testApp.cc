/*
 * testApp.cpp
 *
 *  Created on: 29-Mar-2017
 *      Author: divya
 */

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <string>
#include <string.h>
//#include "EventList.h"
//#include "MyTimer.h"


#include <unistd.h>

using namespace std;

void extractWordsFromString( string& text, vector<string>& tokenList)
{
	// Wrap text in stream.
	std::istringstream myStringStream{text};
	// Read tokens from stream into vector (split at whitespace).
	//	std::vector<std::string> words{std::istream_iterator<std::string>{myStringStream}, std::istream_iterator<std::string>{}};
	//tokenList{std::istream_iterator<std::string>{myStringStream}, std::istream_iterator<std::string>{} };


	std::copy(std::istream_iterator<std::string>(myStringStream),std::istream_iterator<std::string>(),back_inserter(tokenList));

}

int main()
{
	//	EventList eventObj;
	int returnCode = -1;

	string inputEvent("14 feb 9:30 Shyam's birthday remind yearly"/* "5 aug Clean Guest Room remind yearly"*/);
	vector <string> receivedInput;

	//  MyTimer timerObj;
	//timerObj.CreateTimer(10,0);


	extractWordsFromString(inputEvent, receivedInput);
	for(auto itr= receivedInput.begin();itr!=receivedInput.end(); itr++)
	{
		cout<<*itr<<endl;
	}

	std::istringstream iss(receivedInput[2]);
	std::string hour;

	while (std::getline(iss, hour, ':'))
	{
		std::cout << hour << std::endl;
	}
	// 	returnCode = eventObj.ParseAndStoreReceivedEvent(receivedInput);

	//   	timerObj.AddEntitiesToBeNotified(&eventObj);

	struct tm mytime, *localTime;
	time_t currentTime;

	memset(&mytime, 0, sizeof(mytime));

	time( &currentTime );                   // Get the current time
	localTime = localtime( &currentTime );

	mytime.tm_year = localTime->tm_year + 1900;
	mytime.tm_mon  = 2;
	mytime.tm_mday = 31;
	cout<<"Time after increment:Month"<<mytime.tm_mon<<"Day"<<mytime.tm_mday<<endl;
	//mytime.tm_mday = mytime.tm_mday + 1;
	mytime.tm_mon = mytime.tm_mon + 1;

    mktime(&mytime);

    cout<<"Time after increment:Month"<<mytime.tm_mon<<"Day"<<mytime.tm_mday<<"Year"<<mytime.tm_year<<endl;

	while(1)
	{
		sleep(1);
	}
	return returnCode;
}
