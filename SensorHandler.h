#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "Common.h"
#include <NewPing.h>


#define ULTRASONIC_FRONT_LEFT 1
#define ULTRASONIC_FRONT_RIGHT 2
#define ULTRASONIC_BACK_LEFT 3
#define ULTRASONIC_BACK_RIGHT 4

#define ULTRASONIC_FRONT_LEFT_TRIG_PIN ???
#define ULTRASONIC_FRONT_LEFT_ECHO_PIN ???

#define ULTRASONIC_FRONT_RIGHT_TRIG_PIN ???
#define ULTRASONIC_FRONT_RIGHT_ECHO_PIN ???

#define ULTRASONIC_BACK_LEFT_TRIG_PIN ???
#define ULTRASONIC_BACK_LEFT_ECHO_PIN ???

#define ULTRASONIC_BACK_RIGHT_TRIG_PIN ???
#define ULTRASONIC_BACK_RIGHT_ECHO_PIN ???

#define ULTRASONIC_MAX_DISTANCE 200


class SensorHandler {

private:

	/*NewPing sonar[SONAR_NUM] = {     // Sensor object array.
		NewPing(41, 42, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
		NewPing(43, 44, MAX_DISTANCE),
		NewPing(45, 20, MAX_DISTANCE),
		NewPing(21, 22, MAX_DISTANCE),
		NewPing(23, 24, MAX_DISTANCE),
		NewPing(25, 26, MAX_DISTANCE),
		NewPing(27, 28, MAX_DISTANCE),
		NewPing(29, 30, MAX_DISTANCE),
		NewPing(31, 32, MAX_DISTANCE),
		NewPing(34, 33, MAX_DISTANCE),
		NewPing(35, 36, MAX_DISTANCE),
		NewPing(37, 38, MAX_DISTANCE),
		NewPing(39, 40, MAX_DISTANCE),
		NewPing(50, 51, MAX_DISTANCE),
		NewPing(52, 53, MAX_DISTANCE)
	};*/

	void echoCheck();
};

#endif
