#ifndef MOTOR_H_
#define MOTOR_H_

void MotorHandling_init(void);

void MotorHandling_setPositionOnX(int position);

void MotorHandling_getAMeasureOnY(int measure);

void MotorHandling_getAMeasureOnPump(int measure, int pump);

void MotorHandling_setInitialPosition();

#endif /* MOTOR_H_ */
