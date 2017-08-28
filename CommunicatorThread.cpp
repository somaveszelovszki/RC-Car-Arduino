#include "CommunicatorThread.hpp"

CommunicatorThread::CommunicatorThread() : PeriodicThread(PT_PERIOD_TIME_COMMUNICATOR) {
	//commSerial = new SoftwareSerial(BLUETOOTh_RX_PIN, BLUETOOTh_TX_PIN);
	commSerial = new AltSoftSerial(0, 0);
}

void CommunicatorThread::initialize() {
	commSerial->begin(BLUETOOTH_BAUD_RATE);

	watchdog->restart();
}

void CommunicatorThread::run() {
	// reads available characters - if command end was found, returns true
	if (receiveChars()) {
		fetchCommand(recvCommand);
		if (recvCommand.isValid()) {
			__available = true;
			invalidCmdCntr = 0;
		} else
			++invalidCmdCntr;
	}

	// resets watchdog timer used for checking if connection timed out
	watchdog->restart();
}

bool CommunicatorThread::receiveChars() {

	// gets number of available characters
	int availableChars = commSerial->available();

	// reads and stores characters one by one
	for (int i = 0; i < availableChars; ++i) {
		char c = commSerial->read();

		recvBuffer += c;

		// if command end character was received, stops character reading and signals caller by returning true
		if (c == COMMAND_END_CHAR)
			return true;
	}

	return false;
}

void CommunicatorThread::fetchCommand(Command& command) {
	// parses command from buffer string
	Command::fromString(recvBuffer, command);

	// clears buffer string
	recvBuffer = "";
}

void CommunicatorThread::onTimedOut() {
	hasTimedOut = true;
}


int CommunicatorThread::sendCommand(const Command& command) {
	return Common::write(*commSerial, command.toString());
}

CommunicatorThread::~CommunicatorThread() {
	delete &commSerial;
}

