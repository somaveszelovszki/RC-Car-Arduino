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

	/** @brief Parses command from buffer string and clears buffer string content.

	@param dest Will contain parsed command.
	@returns Boolean value indicating if command string was valid.
	*/
	bool fetchCommand(Command& dest);

	void __initialize() override;
	void __run() override;
	void __onTimedOut() override;

public:

	CommunicatorThread();

	Command getCommand();

	/*
		Sends command as string.
		@returns number of bytes sent
	*/
	int sendCommand(const Command& command);

	~CommunicatorThread();
};

#endif	// COMMUNICATOR_THREAD_HPP
