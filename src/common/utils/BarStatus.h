
#ifndef STATUS_H_
#define STATUS_H_

typedef enum eBarStatus
{
  NON_EXECUTING = 0,
  ERROR_WHILE_EXECUTING = 1,
  WAITING_FOR_EXECUTING = 2,
  WAITING_FOR_POWER = 3,
} eBarStatus_t;

#endif // STATUS_H_
