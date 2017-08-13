#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include "Command.hpp"
#include "PeriodicThread.hpp"
#include "Watchdog.hpp"


#define BLUETOOTH_RX_PIN 6
#define BLUETOOTH_TX_PIN 13

#define BLUETOOTH_BAUD_RATE 9600
#define WATCHDOG_TIMEOUT 1000

/*
	Handles communication with phone (through Bluetooth).
*/
class Communicator : public PeriodicThread {
private:
	//SoftwareSerial *commSerial;
	AltSoftSerial *commSerial;

	String recvBuffer = "";

	Watchdog *watchdog = new Watchdog(WATCHDOG_TIMEOUT);	// timeout in ms

	static const unsigned long DEFAULT_CYCLE_PERIOD = 1;

public:

	Communicator();

	/*
		Intializes connection.
	*/
	void initialize();

	/*
		Reads characters from stream.
		@returns true if command end character received - in this case buffer string contains command
	*/
	bool receiveChars();

	/*
		Parses command from buffer string and clears buffer string content.
		@returns pointer to parsed command
	*/
	Command *fetchCommand();
	
	/*
		Sends command as string.
		@returns number of bytes sent
	*/
	size_t sendCommand(const Command& command);

	/*
		Gets watchdog.
	*/
	Watchdog *getWatchdog() const;

	~Communicator();
};

#endif	// COMMUNICATOR_HPP
