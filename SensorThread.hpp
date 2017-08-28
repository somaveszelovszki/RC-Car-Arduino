#ifndef SENSOR_THREAD_HPP
#define SENSOR_THREAD_HPP

#include "PeriodicThread.hpp"
#include "Ultrasonic.hpp"

/*
	Reads, validates and stores sensors' data.
*/
class SensorThread : public PeriodicThread {
private:

	void onTimedOut();

	void initialize();

public:
	Ultrasonic *ultrasonic;

	SensorThread();

	void run();
};

void ultrasonicEchoCheckIT();

#endif	// SENSOR_THREAD_HPP
