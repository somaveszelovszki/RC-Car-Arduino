#ifndef RC_CAR__CONFIG__HPP
#define RC_CAR__CONFIG__HPP

#include <Arduino.h>
#include <stdint.h>

/** @brief Configurations - car specifications, task period times, communication parameters, Arduino pinout, etc.
*/

#define __DEBUG__   1

// Car specifications [cm]

#define CAR_PIVOT_DIST_FRONT_REAR   20.0f        // distance between front and rear pivots
#define CAR_PIVOT_DIST_MID          (CAR_PIVOT_DIST_FRONT_REAR / 2)        // distance between pivot and car middle

#define CAR_PIVOT_LENGTH            8.0f            // length of front and rear pivots (middle to wheel)

#define CAR_PIVOT_FRONT_DIST        7.2f        // distance between car front and front pivot
#define CAR_PIVOT_REAR_DIST         7.2f        // distance between car rear and rear pivot

#define CAR_WHEEL_CIRC              20.3f        // circumference of motor-powered wheels [cm]

#define CAR_WIDTH                   (2 * CAR_PIVOT_LENGTH)
#define CAR_HEIGHT                  (CAR_PIVOT_FRONT_DIST + CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_REAR_DIST)

// Periodic tasks

#define TASK_COUNT  4

// frequencies [ms]

#define TASK_PERIOD_TIME_ULTRASONIC         15
#define TASK_PERIOD_TIME_COMMUNICATOR       10
#define TASK_PERIOD_TIME_DRIVE              5
#define TASK_PERIOD_TIME_ROT                15

// watchdog timeouts [ms]

#define TASK_WATCHDOG_TIMEOUT_ULTRASONIC    100
#define TASK_WATCHDOG_TIMEOUT_COMMUNICATOR  1000
#define TASK_WATCHDOG_TIMEOUT_DRIVE         50
#define TASK_WATCHDOG_TIMEOUT_ROTARY        200

// Watchdog

#define WD_MAX_NUM_WATCHDOGS        TASK_COUNT

// Communication

#define COMM_RX_PIN                 0
#define COMM_TX_PIN                 1
#define COMM_EN_SIGNAL_LEVEL        LOW
#define COMM_BLUETOOTH_BAUD_RATE    9600

#define COMM_MSG_SEPARATOR_LENGTH   4
#define COMM_MSG_CODE_LENGTH        1
#define COMM_MSG_DATA_LENGTH        4
#define COMM_MSG_LENGTH             (COMM_MSG_SEPARATOR_LENGTH + COMM_MSG_CODE_LENGTH + COMM_MSG_DATA_LENGTH)

#define COMM_MSG_SEPARATOR          static_cast<int32_t>(0x7fffffff)    // forbidden as data value

// Trajectory
#define TRAJ_UPDATE_TIMEOUT                     50      // [ms]

// DriveTask
#define DRIVE_ENV_GRID_UPDATE_TIMEOUT           100     // [ms]
#define DRIVE_MSG_WATCHDOG_TIMEOUT              1000    // [ms]
#define DRIVE_FORCE_STEERING_TIME               500     // [ms]
#define DRIVE_FORCE_STOP_TIME                   3000    // [ms]
#define DRIVE_PRE_CRASH_TIME_FORCE_STOP         0.5f    // critical time until crash for force stop - in [sec]
#define DRIVE_PRE_CRASH_TIME_DRIVE_CMD_DISABLE  0.8f    // critical time until crash for command disable - in [sec]

// minimum distance to keep from obstacles - in [cm]
// if trajectory is within this distance from an obstacle, will be handled as if a crash was going to happen
#define MIN_OBSTACLE_DIST       2.0f

// MotorHandler

// ServoMotor
#define SERVO_PIN   9

#define DC_MOTOR_FORWARD_PIN    5
#define DC_MOTOR_BACKWARD_PIN   6

// steering construction restrictions
#define STEER_MAX           0.698131701f    // 40 degrees - in [rad]

// Transfer rate between wheel steering angle and servo angle
#define STEER_TO_SERVO_TR   (-1.6f)

#define SERVO_POS_MID_DEG   80  // in [degree]

// DCMotor
#define DC_MAX_value        255
#define DC_MIN_value        (-DC_MAX_value)

#define DC_MAX_SPEED        55.0f    // [cm/sec]

#define DC_ZERO_SPEED_BOUNDARY           1.0f

// speed controller
#define DC_CONTROL_UPDATE_PERIOD_TIME    10
#define DC_CONTROL_Kp                    0.5f
#define DC_CONTROL_Kd                    0.1f

// motor transfer rates

#define MOTOR_TO_ROTARY_TR   1.0f    // transfer rate between motor and rotary encoder
#define MOTOR_TO_WHEEL_TR    1.0f    // transfer rate between motor and wheels

// Rotary encoder

#define ROT_EN_PIN      A5

#define ROT_D0_PIN      2
#define ROT_D1_PIN      4
#define ROT_D2_PIN      3
#define ROT_D3_PIN      A0
#define ROT_D4_PIN      A1
#define ROT_D5_PIN      A2
#define ROT_D6_PIN      A3
#define ROT_D7_PIN      A4

// Rotary resolution
#define ROT_RES         600

// ROtary evaluation multiplier
#define ROT_EVAL_MUL    4

// rotary counter resolution
#define ROT_COUNTER_RES 256

// Defines maximum previous position change for position-validation:
//        if the difference between the new position and the previous is lower than this value,
//        then it will be accepted without any overflow-check
#define ROT_OVERFLOW_PREV_MAX_D_POS     128

// Ultrasonic sensors

#define ULTRA_TRIGGER_PIN   12
#define ULTRA_ECHO_PIN      13

#define ULTRA_SEL_0_PIN     8
#define ULTRA_SEL_1_PIN     7
#define ULTRA_SEL_2_PIN     11
#define ULTRA_SEL_3_PIN     10

#define ULTRA_NUM_SENSORS           12
#define ULTRA_MAX_DIST              200u
#define ULTRA_ECHO_TIMEOUT          10

// defines how many stored measured values need to be ULTRA_MAX_DIST, so that we can validate value
//     ->    many times the sensors do not respond, which equals ULTRA_MAX_DIST as a value,
//         so we must not believe it unless the result is the same for a few times in a row
#define ULTRA_VALID_MAX_DIST_SAMPLE_NUM     5   // -> if the result has been ULTRA_MAX_DIST for 2 times in a row, we believe it
#define ULTRA_VALID_MAX_DIST_REL_ERR        0
#define ULTRA_VALID_DEF_SAMPLE_NUM          2
#define ULTRA_VALID_DEF_REL_ERR             0.5f

#define ULTRA_NUM_DIST_SAMPLES    max(ULTRA_VALID_MAX_DIST_SAMPLE_NUM, ULTRA_VALID_DEF_SAMPLE_NUM)

// defines how many times in a row an ultrasonic sensor can skip response before being marked unresponsive
// -> unresponsive sensors will not be pinged again
#define ULTRA_VALID_MAX_NON_RESPONSIVE_COUNT    10

// Sensor positions and view angles
// view angle defines the angle of the sensor relative to the RIGHT direction
#define __ULTRA_VIEW_ANGLE          static_cast<float>(18.435f * DEG_TO_RAD)

#define __ULTRA_POS_X_MID           2.4f
#define __ULTRA_POS_Y_MID           CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_CORNER        7.2f
#define __ULTRA_POS_Y_CORNER        CAR_PIVOT_DIST_MID

#define __ULTRA_POS_X_SIDE          (CAR_PIVOT_LENGTH / 2)
#define __ULTRA_POS_Y_SIDE_FRONT    2.0f
#define __ULTRA_POS_Y_SIDE_REAR     3.6f



#define ULTRA_POS_X_RF          __ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RF          __ULTRA_POS_Y_SIDE_FRONT
#define ULTRA_VIEW_ANGLE_RF     static_cast<float>(-M_PI_2 + __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_RF            11

#define ULTRA_POS_X_FRC         __ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_FRC         __ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FRC    (-__ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_FRC           12

#define ULTRA_POS_X_FR          __ULTRA_POS_X_MID
#define ULTRA_POS_Y_FR          __ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FR     0.0f
#define ULTRA_IDX_FR            1

#define ULTRA_POS_X_FL          (-__ULTRA_POS_X_MID)
#define ULTRA_POS_Y_FL          __ULTRA_POS_Y_MID
#define ULTRA_VIEW_ANGLE_FL     0.0f
#define ULTRA_IDX_FL            2

#define ULTRA_POS_X_FLC         (-__ULTRA_POS_X_CORNER)
#define ULTRA_POS_Y_FLC         __ULTRA_POS_Y_CORNER
#define ULTRA_VIEW_ANGLE_FLC    __ULTRA_VIEW_ANGLE
#define ULTRA_IDX_FLC            3



#define ULTRA_POS_X_LF          (-__ULTRA_POS_X_SIDE)
#define ULTRA_POS_Y_LF          __ULTRA_POS_Y_SIDE_FRONT
#define ULTRA_VIEW_ANGLE_LF     static_cast<float>(M_PI_2 - __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_LF            4

#define ULTRA_POS_X_LR          (-__ULTRA_POS_X_SIDE)
#define ULTRA_POS_Y_LR          __ULTRA_POS_Y_SIDE_REAR
#define ULTRA_VIEW_ANGLE_LR     static_cast<float>(M_PI_2 + __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_LR            5



#define ULTRA_POS_X_RLC         (-__ULTRA_POS_X_CORNER)
#define ULTRA_POS_Y_RLC         (-__ULTRA_POS_Y_CORNER)
#define ULTRA_VIEW_ANGLE_RLC    static_cast<float>(M_PI - __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_RLC           6

#define ULTRA_POS_X_RL          (-__ULTRA_POS_X_MID)
#define ULTRA_POS_Y_RL          (-__ULTRA_POS_Y_MID)
#define ULTRA_VIEW_ANGLE_RL     static_cast<float>(M_PI)
#define ULTRA_IDX_RL            7

#define ULTRA_POS_X_RR          __ULTRA_POS_X_MID
#define ULTRA_POS_Y_RR          (-__ULTRA_POS_Y_MID)
#define ULTRA_VIEW_ANGLE_RR     static_cast<float>(M_PI)
#define ULTRA_IDX_RR            8

#define ULTRA_POS_X_RRC         __ULTRA_POS_X_CORNER
#define ULTRA_POS_Y_RRC         (-__ULTRA_POS_Y_CORNER)
#define ULTRA_VIEW_ANGLE_RRC    static_cast<float>(-M_PI + __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_RRC           13



#define ULTRA_POS_X_RiR         __ULTRA_POS_X_SIDE
#define ULTRA_POS_Y_RiR         __ULTRA_POS_Y_SIDE_REAR
#define ULTRA_VIEW_ANGLE_RiR    static_cast<float>(-M_PI_2 - __ULTRA_VIEW_ANGLE)
#define ULTRA_IDX_RiR           10

// Environment

#define ENV_SECTIONS_NUM            3
#define ENV_SECTION_POINTS_MAX_NUM  5
#define ENV_SECTION_POINTS_DIST     10.0f    // [cm]

// Absolute position is measured from the start position
// (the moment the car is powered on, its position is saved as absolute origo)
#define ENV_GRID_DIST               (CAR_WIDTH / 2)     // distance between stored absoulte points [cm]
#define ENV_GRID_AXIS_NUM_POINTS    64                  // number of absolute points stored in a direction (x, y)
#define ENV_GRID_POINT_BIT_DEPTH    1                   // bit depth of the environment points
#define ENV_CAR_GRID_DIST_RATE      4                   // defines car grid resolution (used when sending car grid coordinates with higher resolution)

#define ENV_ABS_SECTION_POINTS_MAX_NUM  20  // max number of grid points in a section

// Array: supported bit depths

#if ENV_GRID_POINT_BIT_DEPTH == 1
#   define ARRAY_1_EN true
#else
#   define ARRAY_1_EN false
#endif // ENV_GRID_POINT_BIT_DEPTH == 1

#if ENV_GRID_POINT_BIT_DEPTH == 2
#   define ARRAY_2_EN true
#else
#   define ARRAY_2_EN false
#endif // ENV_GRID_POINT_BIT_DEPTH == 2

#if ENV_GRID_POINT_BIT_DEPTH == 4
#   define ARRAY_4_EN true
#else
#   define ARRAY_4_EN false
#endif // ENV_GRID_POINT_BIT_DEPTH == 4


#endif // RC_CAR__CONFIG__HPP
