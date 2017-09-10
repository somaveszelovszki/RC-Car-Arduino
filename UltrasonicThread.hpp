#ifndef ULTRA_THREAD_HPP
#define ULTRA_THREAD_HPP

#include "PeriodicThread.hpp"
#include "Ultrasonic.hpp"

/*
	Reads, validates and stores sensors' data.
*/
class UltrasonicThread : public PeriodicThread {
private:
	Ultrasonic ultrasonic;

	void __initialize() override;
	void __run() override;
	void __onTimedOut() override;

public:
	UltrasonicThread();

	void getSensedPoints(Point<float> dest[ULTRA_NUM_SENSORS]);

	friend void ultrasonicEchoCheckIT();
};

#endif	// ULTRA_THREAD_HPP
