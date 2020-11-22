#ifndef MOTORS_H_
#define MOTORS_H_

// MOTOR PINS OUT
#define RIGHT_MOTOR_FORWARD 0                      // Port B pin 0, TPM1_CH0,  
#define RIGHT_MOTOR_REVERSE 1                      // Port B pin 1, TPM1_CH1,
#define LEFT_MOTOR_FORWARD  2                      // Port B pin 2, TPM2_CH0,
#define LEFT_MOTOR_REVERSE  3                      // Port B pin 3, TPM2_CH1

// PWM SETTINGS
#define FAST 100		// Used when going forward and backward
#define SLOW 80 		// Used when making turns

// DIRECTION SETTINGS
#define STOP			0
#define FORWARD 		1
#define REVERSE 		2
#define LEFT			3
#define RIGHT			4

void initMotors(void);
int getPWM(int);
void move(int);

#endif
