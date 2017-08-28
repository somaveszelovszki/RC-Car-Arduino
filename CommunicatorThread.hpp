#ifndef COMMUNICATOR_THREAD_HPP
#define COMMUNICATOR_THREAD_HPP

//#include <SoftwareSerial.h>
#include "AltSoftSerial.h"
#include "Command.hpp"
#include "PeriodicThread.hpp"
#include "Watchdog.hpp"

/*
	Handles communication with phone (through Bluetooth).
*/
class CommunicatorThread : public PeriodicThread {
private:
	//SoftwareSerial *commSerial;
	AltSoftSerial *commSerial;

	String recvBuffer = "";

	Command recvCommand;
	int invalidCmdCntr = 0;
	bool __available = false;
	bool hasTimedOut = false;	// TODO find a more elegant and effective way

	/*
	Reads characters from stream.
	@returns true if command end character received - in this case buffer string contains command
	*/
	bool receiveChars();

	/*
	Parses command from buffer string and clears buffer string content.
	@returns pointer to parsed command
	*/
	void fetchCommand(Command& command);

	void onTimedOut();

	/*
	Intializes connection.
	*/
	void initialize();

public:

	CommunicatorThread();

	void run();

	bool available() {
		return __available;
	}

	const Command* getCommand() {
		__available = false;
		return hasTimedOut ? static_cast<const Command*>(NULL) : &recvCommand;
	}

	/*
		Sends command as string.
		@returns number of bytes sent
	*/
	int sendCommand(const Command& command);

	~CommunicatorThread();
};

#endif	// COMMUNICATOR_THREAD_HPP
