#ifndef DEF_HPP
#define DEF_HPP

/*
	Definitions used throughout the application.
*/

// Car specifications [cm]

#define CAR_PIVOT_DIST_FRONT_REAR	10.0f		// distance between front and rear pivots
#define CAR_PIVOT_DIST_MID		(CAR_PIVOT_DIST_FRONT_REAR / 2)		// distance between pivot and car middle

#define CAR_PIVOT_LENGTH_FRONT			5.0f			// length of front pivot (middle to wheel)
#define CAR_PIVOT_LENGTH_REAR			5.0f			// length of rear pivot (middle to wheel)

#define CAR_PIVOT_LENGTH_MID ((CAR_PIVOT_LENGTH_FRONT + CAR_PIVOT_LENGTH_REAR) / 2)

#define CAR_PIVOT_FRONT_DISTANCE		10.0f		// distance between car front and front pivot
#define CAR_PIVOT_REAR_DISTANCE			10.0f		// distance between car rear and rear pivot

// Periodic threads

#define PT_MAX_NUM_THREADS 4

	// frequencies [ms]

#define PT_PERIOD_TIME_ULTRASONIC 1
#define PT_PERIOD_TIME_COMMUNICATOR 1
#define PT_PERIOD_TIME_DRIVE 1
#define PT_PERIOD_TIME_ROTARY 50

	// watchdog timeouts [ms]

#define PT_WATCHDOG_TIMEOUT_ULTRASONIC 10
#define PT_WATCHDOG_TIMEOUT_COMMUNICATOR 1000
#define PT_WATCHDOG_TIMEOUT_DRIVE 20
#define PT_WATCHDOG_TIMEOUT_ROTARY 200

// Watchdog

#define WD_MAX_NUM_WATCHDOGS PT_MAX_NUM_THREADS

// Command

#define COMMAND_END_CHAR ';'
#define COMMAND_SEPARATOR_CHAR ':'

// Communication

#define BLUETOOTH_RX_PIN 6
#define BLUETOOTH_TX_PIN 13

#define BLUETOOTH_BAUD_RATE 9600

// DriveThread

#define DRIVE_FORCE_STOP_TIME 3000	// [ms]
#define CRITICAL_PRE_CRASH_TIME 0.5		// critical time until crash - in [sec]


// minimum distance to keep from obstacles - in [cm]
// if trajectory is within this distance from an obstacle, will be handled as if a crash was going to happen
#define MIN_OBSTACLE_DISTANCE 2.0f

// MotorHandler

	// ServoMotor

// servo pin
#define SERVO_PIN 9

#define DC_MOTOR_FORWARD_PIN 3
#define DC_MOTOR_BACKWARD_PIN 11

// servo command value range (0 is servo middle position)
#define SERVO_COMMAND_VALUE_MIN -100
#define SERVO_COMMAND_VALUE_MAX 100

// servo motor restrictions
#define SERVO_POSITION_MIN 0
#define SERVO_POSITION_MAX 180

// steering construction restriction
#define SERVO_DEFAULT_MAX_ROTATION 70

	// DCMotor
#define DC_MAX_VALUE 255
#define DC_MIN_VALUE (-1) * DC_MAX_VALUE

#define DC_MAX_SPEED 55 // [cm/sec]

// speed controller
#define DC_CONTROL_UPDATE_PERIOD_TIME 10
#define DC_CONTROL_Kp 2.0f
#define DC_CONTROL_Kd 2.0f

// motor transfer rates

#define MOTOR_ROTARY_TRANSFER_RATE 3.0f	// transfer rate between motor and rotary encoder
#define MOTOR_WHEEL_TRANSFER_RATE 1.0f	// transfer rate between motor and wheels
#define WHEEL_CIRCUMFERENCE 20.3f		// circumference of motor-powered wheels [cm]

// Rotary encoder

#define ROTARY_EN_PIN 0

#define ROTARY_D0_PIN 0
#define ROTARY_D1_PIN 0
#define ROTARY_D2_PIN 0
#define ROTARY_D3_PIN 0
#define ROTARY_D4_PIN 0
#define ROTARY_D5_PIN 0
#define ROTARY_D6_PIN 0
#define ROTARY_D7_PIN 0

#define ROTARY_RESOLUTION 100

// Defines maximum previous position change for position-validation:
//		if the difference between the new position and the previous is lower than this value,
//		then it will be accepted without any overflow-check
#define ROTARY_OVERFLOW_PREV_MAX_D_POS 20

// Ultrasonic sensors

#define ULTRA_TRIGGER_PIN A4
#define ULTRA_ECHO_PIN ULTRA_TRIGGER_PIN

#define ULTRA_SEL_0_PIN A0
#define ULTRA_SEL_1_PIN A1
#define ULTRA_SEL_2_PIN A2
#define ULTRA_SEL_3_PIN A3

#define ULTRA_NUM_SENSORS 12
#define ULTRA_NUM_DIST_SAMPLES 10
#define ULTRA_MAX_DISTANCE 200

// defines how many stored measured values need to be ULTRA_MAX_DISTANCE, so that we can validate value
// 	->	many times the sensors do not respond, which equals ULTRA_MAX_DISTANCE as a value,
// 		so we must not believe it unless the result is the same for a few times in a row
#define ULTRA_VALID_MAX_DIST_SAMPLE_NUM 5		// -> if the result has been ULTRA_MAX_DISTANCE for 2 times in a row, we believe it
#define ULTRA_VALID_MAX_DIST_RELATIVE_ERROR 0
#define ULTRA_VALID_DEFAULT_SAMPLE_NUM 2
#define ULTRA_VALID_DEFAULT_RELATIVE_ERROR 0.5F

// defines how many times in a row an ultrasonic sensor can skip response before being marked unresponsive
// -> unresponsive sensors will not be pinged again
#define ULTRA_VALID_MAX_NON_RESPONSIVE_COUNT 10

	// Sensor positions and view angles
// TODO update this number, I just made it up
// view angle defines the angle of the sensor relative to the RIGHT direction
#define __ULTRA_VIEW_ANGLE 0.5f

#define __ULTRA_POS_X_MID 3.0f
#define __ULTRA_POS_Y_MID CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_CORNER 8.0f
#define __ULTRA_POS_Y_CORNER CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_SIDE (CAR_PIVOT_LENGTH_MID / 2)
#define __ULTRA_POS_Y_SIDE 3.0f



#define ULTRA_POS_X_FLC -__ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_FLC __ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FLC (M_PI_2 + __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_FL -__ULTRA_POS_X_MID
#define ULTRA_POS_Y_FL __ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FL M_PI_2

#define ULTRA_POS_X_FR __ULTRA_POS_X_MID
#define ULTRA_POS_Y_FR __ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FR M_PI_2

#define ULTRA_POS_X_FRC __ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_FRC __ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FRC (M_PI_2 - __ULTRA_VIEW_ANGLE)



#define ULTRA_POS_X_RLC -__ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_RLC -__ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_RLC (-M_PI_2 - __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_RL -__ULTRA_POS_X_MID
#define ULTRA_POS_Y_RL -__ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_RL -M_PI_2

#define ULTRA_POS_X_RR __ULTRA_POS_X_MID
#define ULTRA_POS_Y_RR -__ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_RR -M_PI_2

#define ULTRA_POS_X_RRC __ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_RRC -__ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_RRC (-M_PI_2 + __ULTRA_VIEW_ANGLE)



#define ULTRA_POS_X_LF -__ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_LF __ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_LF (M_PI - __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_LR -__ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_LR __ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_LR (M_PI + __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_RF __ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RF __ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_RF __ULTRA_VIEW_ANGLE

#define ULTRA_POS_X_RiR __ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RiR __ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_RiR -__ULTRA_VIEW_ANGLE

#endif		// DEF_HPP