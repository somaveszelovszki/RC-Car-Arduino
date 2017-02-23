#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <SoftwareSerial.h>
#include "Command.h"

/*
	Handles communication with phone (through BlueTooth).
*/
class Communicator {
private:

	const int BLUETOOTH_BAUD_RATE = 9600;

	SoftwareSerial* bluetoothSerial;

	String recvBuffer = "";
	int recv_watchdog_timeout = 3000;
	int recv_watchdog = 0;


public:

	Communicator();

	void initialize();

	bool receiveChar();

	Command* fetchCommand();
	
	size_t sendCommand(Command* command);

	bool timedOut();

	void reset_recv_watchdog();

	~Communicator();
};


#endif
