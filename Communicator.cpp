#include "Communicator.hpp"

Communicator::Communicator() : PeriodicThread(DEFAULT_CYCLE_PERIOD) {
	//commSerial = new SoftwareSerial(BLUETOOTh_RX_PIN, BLUETOOTh_TX_PIN);
	commSerial = new AltSoftSerial(0, 0);
}

void Communicator::initialize() {
	commSerial->begin(BLUETOOTH_BAUD_RATE);

	watchdog->restart();
}

bool Communicator::receiveChars() {

	// gets number of available characters
	int availableChars = commSerial->available();

	// reads and stores characters one by one
	for (int i = 0; i < availableChars; ++i) {
		char c = commSerial->read();

		recvBuffer += c;

		// if command end character was received, stops character reading and signals caller by returning true
		if (c == COMMAND_END_CHAR) {
			return true;
		}
	}

	return false;
}

Command *Communicator::fetchCommand() {
	// parses command from buffer string
	Command *command = Command::fromString(recvBuffer);

	// clears buffer string
	recvBuffer = "";

	return command;
}


size_t Communicator::sendCommand(const Command& command) {
	return Common::write(*commSerial, command.toString());
}

Watchdog *Communicator::getWatchdog() const {
	return watchdog;
}

Communicator::~Communicator() {
	delete &commSerial;
}

