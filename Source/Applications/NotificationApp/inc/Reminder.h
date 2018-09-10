/**
 * @file
 * Defines the helper functions for reminder feature/Event notification
 */

#ifndef INC_REMINDERFUNCTIONS_H_
#define INC_REMINDERFUNCTIONS_H_

class ReminderFunctions
{
  long remainingTime;
public:
  ReminderFunctions();
  long CalculateRemainingTime( struct tm* remindTime);
};




#endif /* INC_REMINDERFUNCTIONS_H_ */
