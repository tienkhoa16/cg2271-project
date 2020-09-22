#ifndef MOTORS_H_
#define MOTORS_H_

// Motor Pins
#define RIGHT_MOTOR_FORWARD	 	0		// PTD0 TPM0 CH0
#define RIGHT_MOTOR_REVERSE		2		//PTD2 TPM0 CH2
#define LEFT_MOTOR_FORWARD		3 	// PTD3 TPM0 CH3
#define LEFT_MOTOR_REVERSE		5		// PTD5 TPM0 CH5

// PWM SETTINGS
#define FAST 100		// Used when going forward
#define SLOW 75			// Used when making turns

// can maybe change to enum
// DIRECTION SETTINGS
#define STOP			0
#define FORWARD 		3
#define REVERSE 		5
#define LEFT			9
#define RIGHT			17
#define F_LEFT			11
#define F_RIGHT			19
#define	R_LEFT			13
#define	R_RIGHT			21

#define MASK_ON(x)	(x & 0x1)

void initMotors(void);

int PWM(int);

void move(int);

#endif