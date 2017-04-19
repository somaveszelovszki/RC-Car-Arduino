#include "Communicator.h"

Communicator::Communicator() : PeriodicThread(DEFAULT_CYCLE_PERIOD) {
	bluetoothSerial = new SoftwareSerial(BLUETOOTH_RX_PIN, BLUETOOTH_TX_PIN);
}

void Communicator::initialize() {
	bluetoothSerial->begin(BLUETOOTH_BAUD_RATE);

	watchdog->restart();
}

bool Communicator::receiveChars() {

	// gets number of available characters
	int availableChars = bluetoothSerial->available();

	// reads and stores characters one by one
	for (int i = 0; i < availableChars; ++i) {
		char c = bluetoothSerial->read();

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
	return Common::write(*bluetoothSerial, command.toString());
}

Watchdog *Communicator::getWatchdog() const {
	return watchdog;
}

Communicator::~Communicator() {
	delete &bluetoothSerial;
}

