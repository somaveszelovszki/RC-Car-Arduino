#include "Communicator.h"

Communicator::Communicator() {
	bluetoothSerial = new SoftwareSerial(Common::PINS::BLUETOOTH_RX, Common::PINS::BLUETOOTH_TX);
}

Communicator::~Communicator() {
	delete bluetoothSerial;
}

void Communicator::initialize() {
	bluetoothSerial->begin(BLUETOOTH_BAUD_RATE);

	this->reset_recv_watchdog();
}

void Communicator::reset_recv_watchdog() {
	recv_watchdog = recv_watchdog_timeout;
}


bool Communicator::receiveChar() {

	if (bluetoothSerial->available()) {

		this->reset_recv_watchdog();

		char c = this->bluetoothSerial->read();

		Serial.print(c);

		if (c == Command::END_CHAR) {
			return true;
		}

		recvBuffer += c;
	}

	return false;
}

Command* Communicator::fetchCommand() {
	Command* command = Command::fromString(recvBuffer);
	recvBuffer = "";

	return command;
}


size_t Communicator::sendCommand(Command* command) {
	return Common::write(bluetoothSerial, command->toString());
}

bool Communicator::timedOut() {
	return recv_watchdog <= 0;
}