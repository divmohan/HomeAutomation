/*
 * CommonTypes.h
 *
 *  Created on: 06-Feb-2017
 *      Author: divya
 */

#ifndef INC_COMMONTYPES_H_
#define INC_COMMONTYPES_H_

#include <string>
#include <vector>
#include <map>

enum class AutomationFunctions: uint8_t
{
 Reminder = 1,
 Entertainment,
 HdwCtrl
};

std::vector<std::string>commands;

std::map<AutomationFunctions,commands>CommandActionMap;

#endif /* INC_COMMONTYPES_H_ */
