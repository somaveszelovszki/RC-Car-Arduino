#ifndef DEF_HPP
#define DEF_HPP

/*
	Definitions used throughout the application.
*/

// Car specifications [cm]

#define CAR_PIVOT_DISTANCE_FRONT_REAR	10.0		// distance between front and rear pivots
#define CAR_PIVOT_DISTANCE_MIDDLE		(CAR_PIVOT_DISTANCE_FRONT_REAR / 2)		// distance between pivot and car middle

#define CAR_PIVOT_LENGTH_FRONT			5.0			// length of front pivot (middle to wheel)
#define CAR_PIVOT_LENGTH_REAR			5.0			// length of rear pivot (middle to wheel)

#define CAR_PIVOT_FRONT_DISTANCE		10.0		// distance between car front and front pivot
#define CAR_PIVOT_REAR_DISTANCE			10.0		// distance between car rear and rear pivot

// Periodic threads

#define PT_MAX_NUM_THREADS 3

	// frequencies [ms]

#define PT_PERIOD_TIME_SENSOR 1
#define PT_PERIOD_TIME_COMMUNICATOR 1
#define PT_PERIOD_TIME_DRIVE 1

	// watchdog timeouts [ms]

#define PT_WATCHDOG_TIMEOUT_SENSOR 10
#define PT_WATCHDOG_TIMEOUT_COMMUNICATOR 1000
#define PT_WATCHDOG_TIMEOUT_DRIVE 20

// Command

#define COMMAND_END_CHAR ';'
#define COMMAND_SEPARATOR_CHAR ':'

// Communication

#define BLUETOOTH_RX_PIN 6
#define BLUETOOTH_TX_PIN 13

#define BLUETOOTH_BAUD_RATE 9600

// Driving

#define EMERGENCY_BREAK_STOP_TIME_MS 3000;
#define CRITICAL_PRE_CRASH_TIME 0.5		// critical time until crash - in [sec]


// minimum distance to keep from obstacles - in [cm]
// if trajectory is within this distance from an obstacle, will be handled as if a crash was going to happen
#define MIN_OBSTACLE_DISTANCE 2.0

// Motor handler

// servo pin
#define SERVO_PIN 9

#define DC_MOTOR_FORWARD_PIN 3
#define DC_MOTOR_BACKWARD_PIN 11

// servo command value range (0 is servo middle position)
#define SERVO_COMMAND_VALUE_MIN -100
#define SERVO_COMMAND_VALUE_MAX 100
#define SERVO_COMMAND_VALUE_MIDDLE (SERVO_COMMAND_VALUE_MIN + (SERVO_COMMAND_VALUE_MAX - SERVO_COMMAND_VALUE_MIN) / 2)

// servo motor restrictions
#define SERVO_POSITION_MIN 0
#define SERVO_POSITION_MAX 180

// steering construction restriction
#define SERVO_DEFAULT_MAX_ROTATION 70

// DC motor command value range (50 is DC motor stop, >50 means FORWARD)
#define DC_COMMAND_VALUE_MIN 0
#define DC_COMMAND_VALUE_MAX 100
#define DC_COMMAND_VALUE_STOP (DC_COMMAND_VALUE_MIN + (DC_COMMAND_VALUE_MAX - DC_COMMAND_VALUE_MIN) / 2)

// DC Motor value range
#define DC_MAX_VALUE 255
#define DC_MIN_VALUE (-1) * DC_MAX_VALUE

#define MAX_SPEED 55 // [cm/sec]

#define MOTOR_ROTARY_TRANSFER_RATE 3.0
#define MOTOR_WHEEL_TRANSFER_RATE 1.0			// transfer rate between motor and wheels (defined by car architecture)
#define WHEEL_CIRCUMFERENCE 20.3		// circumference of motor-powered wheels - in [cm]

#define SPEED_CONTROLLER_Kp 2
#define SPEED_CONTROLLER_Kd 2

// Rotary encoder

#define ROTARY_ENCODER_A_PIN 0
#define ROTARY_ENCODER_B_PIN 1

#define ROTARY_RESOLUTION 20

// Ultrasonic sensors

#define ULTRASONIC_TRIGGER_PIN A4
#define ULTRASONIC_ECHO_PIN ULTRASONIC_TRIGGER_PIN

#define ULTRASONIC_SEL_0_PIN A0
#define ULTRASONIC_SEL_1_PIN A1
#define ULTRASONIC_SEL_2_PIN A2
#define ULTRASONIC_SEL_3_PIN A3

#define ULTRASONIC_NUM_SENSORS 12
#define ULTRASONIC_NUM_DISTANCE_SAMPLES 10
#define ULTRASONIC_MAX_DISTANCE 200

// defines how many stored measured values need to be ULTRASONIC_MAX_DISTANCE, so that we can validate value
// 	->	many times the sensors do not respond, which equals ULTRASONIC_MAX_DISTANCE as a value,
// 		so we must not believe it unless the result is the same for a few times in a row
#define VALIDATION_MAX_DIST_SAMPLE_NUM 5		// -> if the result has been ULTRASONIC_MAX_DISTANCE for 2 times in a row, we believe it
#define VALIDATION_MAX_DIST_RELATIVE_ERROR 0
#define VALIDATION_DEFAULT_SAMPLE_NUM 2
#define VALIDATION_DEFAULT_RELATIVE_ERROR 0.5F

#endif		// DEF_HPP
