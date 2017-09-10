#include "CommunicatorThread.hpp"

CommunicatorThread::CommunicatorThread() : PeriodicThread(PT_PERIOD_TIME_COMMUNICATOR, PT_WATCHDOG_TIMEOUT_COMMUNICATOR) {
	//commSerial = new SoftwareSerial(BLUETOOTh_RX_PIN, BLUETOOTh_TX_PIN);
	commSerial = new AltSoftSerial(0, 0);
}

void CommunicatorThread::__initialize() {
	commSerial->begin(BLUETOOTH_BAUD_RATE);

	watchdog->restart();
}

void CommunicatorThread::__run() {
	if (!hasTimedOut) {
		// reads available characters - if command end was found, returns true
		if (receiveChars()) {
			if (fetchCommand(recvCommand)) {
				__available = true;
				invalidCmdCntr = 0;
			} else
				++invalidCmdCntr;
		}

		// resets watchdog timer used for checking if connection timed out
		watchdog->restart();
	}
}

void CommunicatorThread::__onTimedOut() {
	hasTimedOut = true;
}

Command CommunicatorThread::getCommand() {
	bool commandAvailable = __available;
	__available = false;
	return commandAvailable ? recvCommand : Command::NoNewCommand;
}

bool CommunicatorThread::receiveChars() {

	// if command end character was received, stops character reading and signals caller by returning true
	bool cmdEndReceived = false;

	// gets number of available characters
	int availableChars = commSerial->available();

	// reads and stores characters one by one
	for (int i = 0; !cmdEndReceived && i < availableChars; ++i) {
		char c = commSerial->read();
		recvBuffer += c;
		cmdEndReceived = (c == COMMAND_END_CHAR);
	}

	return cmdEndReceived;
}

bool CommunicatorThread::fetchCommand(Command& dest) {

	bool valid = Command::isValid(recvBuffer);

	// parses command from buffer string
	if (valid) Command::fromString(recvBuffer, dest);

	// clears buffer string
	recvBuffer = "";

	return valid;
}

int CommunicatorThread::sendCommand(const Command& command) {
	return Common::write(*commSerial, command.toString());
}

CommunicatorThread::~CommunicatorThread() {
	delete commSerial;
}

