/*
 * CommInterface.h
 *
 *  Created on: 04-Feb-2017
 *      Author: divya
 */

#ifndef INC_COMMINTERFACE_H_
#define INC_COMMINTERFACE_H_

#include <sstream>

//!Base class for communicating with the user application [web,phone etc]
class CommInterface
{

	AutoCommon homeAutoObj;
public:
 //should support text, voice, gesture etc

	//pass it to homeAutoClass
  void FetchInputAndProcess(std::stringstream& inputString);

};



#endif /* INC_COMMINTERFACE_H_ */
