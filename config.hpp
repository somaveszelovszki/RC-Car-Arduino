#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace rc_car {

/** @brief Configurations - car specifications, task period times, communication parameters, Arduino pinout, etc.
*/

// Car specifications [cm]

#define CAR_PIVOT_DIST_FRONT_REAR	20.0f		// distance between front and rear pivots
#define CAR_PIVOT_DIST_MID		(CAR_PIVOT_DIST_FRONT_REAR / 2)		// distance between pivot and car middle

#define CAR_PIVOT_LENGTH_FRONT			8.0f			// length of front pivot (middle to wheel)
#define CAR_PIVOT_LENGTH_REAR			8.0f			// length of rear pivot (middle to wheel)

#define CAR_PIVOT_LENGTH_MID ((CAR_PIVOT_LENGTH_FRONT + CAR_PIVOT_LENGTH_REAR) / 2)

#define CAR_PIVOT_FRONT_DISTANCE		7.2f		// distance between car front and front pivot
#define CAR_PIVOT_REAR_DISTANCE			7.2f		// distance between car rear and rear pivot

// Periodic tasks

#define PT_MAX_NUM_TASKS				4

	// frequencies [ms]

#define PT_PERIOD_TIME_ULTRASONIC		1
#define PT_PERIOD_TIME_COMMUNICATOR		1
#define PT_PERIOD_TIME_DRIVE			1
#define PT_PERIOD_TIME_ROTARY			50

	// watchdog timeouts [ms]

#define PT_WATCHDOG_TIMEOUT_ULTRASONIC		10
#define PT_WATCHDOG_TIMEOUT_COMMUNICATOR	1000
#define PT_WATCHDOG_TIMEOUT_DRIVE			20
#define PT_WATCHDOG_TIMEOUT_ROTARY			200

// Watchdog

#define WD_MAX_NUM_WATCHDOGS PT_MAX_NUM_TASKS


// Communication

#define COMM_BLUETOOTH_RX_PIN		6
#define COMM_BLUETOOTH_TX_PIN		13

#define COMM_BLUETOOTH_BAUD_RATE	9600

#define COMM_MESSAGE_SIZE			5

// DriveTask

#define DRIVE_FORCE_STOP_TIME		3000	// [ms]
#define CRITICAL_PRE_CRASH_TIME		0.5		// critical time until crash - in [sec]


// minimum distance to keep from obstacles - in [cm]
// if trajectory is within this distance from an obstacle, will be handled as if a crash was going to happen
#define MIN_OBSTACLE_DISTANCE		2.0f

// MotorHandler

	// ServoMotor

// servo pin
#define SERVO_PIN 9

#define DC_MOTOR_FORWARD_PIN		3
#define DC_MOTOR_BACKWARD_PIN		11

// steering construction restrictions
#define __SERVO_ROT_MAX		60
#define __SERVO_POS_MID		90
#define SERVO_POS_MIN		__SERVO_POS_MID - __SERVO_ROT_MAX
#define SERVO_POS_MAX		__SERVO_POS_MID + __SERVO_ROT_MAX

	// DCMotor
#define DC_MAX_VALUE		255
#define DC_MIN_VALUE		(-DC_MAX_VALUE)

#define DC_MAX_SPEED		55 // [cm/sec]

// speed controller
#define DC_CONTROL_UPDATE_PERIOD_TIME	10
#define DC_CONTROL_Kp					2.0f
#define DC_CONTROL_Kd					2.0f

// motor transfer rates

#define MOTOR_ROTARY_TRANSFER_RATE	3.0f	// transfer rate between motor and rotary encoder
#define MOTOR_WHEEL_TRANSFER_RATE	1.0f	// transfer rate between motor and wheels
#define WHEEL_CIRCUMFERENCE			20.3f		// circumference of motor-powered wheels [cm]

// Rotary encoder

#define ROTARY_EN_PIN		0

#define ROTARY_D0_PIN		0
#define ROTARY_D1_PIN		0
#define ROTARY_D2_PIN		0
#define ROTARY_D3_PIN		0
#define ROTARY_D4_PIN		0
#define ROTARY_D5_PIN		0
#define ROTARY_D6_PIN		0
#define ROTARY_D7_PIN		0

#define ROTARY_RESOLUTION	100

// Defines maximum previous position change for position-validation:
//		if the difference between the new position and the previous is lower than this value,
//		then it will be accepted without any overflow-check
#define ROTARY_OVERFLOW_PREV_MAX_D_POS		20

// Ultrasonic sensors

#define ULTRA_TRIGGER_PIN	A4
#define ULTRA_ECHO_PIN		ULTRA_TRIGGER_PIN

#define ULTRA_SEL_0_PIN		A0
#define ULTRA_SEL_1_PIN		A1
#define ULTRA_SEL_2_PIN		A2
#define ULTRA_SEL_3_PIN		A3

#define ULTRA_NUM_SENSORS		12
#define ULTRA_NUM_DIST_SAMPLES	10
#define ULTRA_MAX_DISTANCE		200

// defines how many stored measured values need to be ULTRA_MAX_DISTANCE, so that we can validate value
// 	->	many times the sensors do not respond, which equals ULTRA_MAX_DISTANCE as a value,
// 		so we must not believe it unless the result is the same for a few times in a row
#define ULTRA_VALID_MAX_DIST_SAMPLE_NUM			5		// -> if the result has been ULTRA_MAX_DISTANCE for 2 times in a row, we believe it
#define ULTRA_VALID_MAX_DIST_RELATIVE_ERROR		0
#define ULTRA_VALID_DEFAULT_SAMPLE_NUM			2
#define ULTRA_VALID_DEFAULT_RELATIVE_ERROR		0.5F

// defines how many times in a row an ultrasonic sensor can skip response before being marked unresponsive
// -> unresponsive sensors will not be pinged again
#define ULTRA_VALID_MAX_NON_RESPONSIVE_COUNT	10

	// Sensor positions and view angles
// TODO update this number, I just made it up
// view angle defines the angle of the sensor relative to the RIGHT direction
#define __ULTRA_VIEW_ANGLE		0.5f

#define __ULTRA_POS_X_MID		3.0f
#define __ULTRA_POS_Y_MID		CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_CORNER	8.0f
#define __ULTRA_POS_Y_CORNER	CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_SIDE		(CAR_PIVOT_LENGTH_MID / 2)
#define __ULTRA_POS_Y_SIDE		3.0f



#define ULTRA_POS_X_FRC			__ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_FRC			__ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FRC	(-__ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_FR			__ULTRA_POS_X_MID
#define ULTRA_POS_Y_FR			__ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FR		0.0f

#define ULTRA_POS_X_FL			(-__ULTRA_POS_X_MID)
#define ULTRA_POS_Y_FL			__ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FL		0.0f

#define ULTRA_POS_X_FLC			(-__ULTRA_POS_X_CORNER)
#define ULTRA_POS_Y_FLC			__ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FLC	__ULTRA_VIEW_ANGLE



#define ULTRA_POS_X_LF			(-__ULTRA_POS_X_SIDE)
#define ULTRA_POS_Y_LF			__ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_LF		static_cast<float>(M_PI_2 - __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_LR			(-__ULTRA_POS_X_SIDE)
#define ULTRA_POS_Y_LR			__ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_LR		static_cast<float>(M_PI_2 + __ULTRA_VIEW_ANGLE)



#define ULTRA_POS_X_RLC			(-__ULTRA_POS_X_CORNER)
#define ULTRA_POS_Y_RLC			(-__ULTRA_POS_Y_CORNER)
#define ULTRA_VIEW_ANGLE_RLC	static_cast<float>(M_PI - __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_RL			(-__ULTRA_POS_X_MID)
#define ULTRA_POS_Y_RL			(-__ULTRA_POS_Y_MID)
#define ULTRA_VIEW_ANGLE_RL		static_cast<float>(M_PI)

#define ULTRA_POS_X_RR			__ULTRA_POS_X_MID
#define ULTRA_POS_Y_RR			(-__ULTRA_POS_Y_MID)
#define ULTRA_VIEW_ANGLE_RR		static_cast<float>(M_PI)

#define ULTRA_POS_X_RRC			__ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_RRC			(-__ULTRA_POS_Y_CORNER)
#define ULTRA_VIEW_ANGLE_RRC	static_cast<float>(-M_PI + __ULTRA_VIEW_ANGLE)



#define ULTRA_POS_X_RiR			__ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RiR			__ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_RiR	static_cast<float>(-M_PI_2 - __ULTRA_VIEW_ANGLE)

#define ULTRA_POS_X_RF			__ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RF			__ULTRA_POS_Y_SIDE
#define ULTRA_VIEW_ANGLE_RF		static_cast<float>(-M_PI_2 + __ULTRA_VIEW_ANGLE)

// Environment

// Number of sensors in a combined section
//#define ENV_COMBINED_SECTION_SENSORS_NUM 3

//#define ENV_COMBINED_SECTIONS_NUM (ULTRA_NUM_SENSORS / ENV_COMBINED_SECTION_SENSORS_NUM)

// Number of environment points surrounding the car
#define ENV_POINTS_NUM			100
#define ENV_POINTS_DELTA_ANGLE	static_cast<float>(2 * M_PI / ENV_POINTS_NUM)

// Absolute position is measured from the start position
// (the moment the car is powered on, its position is saved as absolute origo)
#define ENV_ABS_POINTS_DIST			10	// distance between stored absoulte points [cm]
#define ENV_ABS_POINTS_NUM_X		50	// number of absolute points stored in x direction
#define ENV_ABS_POINTS_NUM_Y		50	// number of absolute points stored in y direction

// Defines maximum number of points in a sensor direction section
//#define ENV_SECTION_POINTS_MAX_NUM 8 
}

#endif		// CONFIG_HPP
