#include "CommunicatorTask.hpp"

using namespace rc_car;

CommunicatorTask::CommunicatorTask() : PeriodicTask(PT_PERIOD_TIME_COMMUNICATOR, PT_WATCHDOG_TIMEOUT_COMMUNICATOR) {
	commSerial = new SoftwareSerial(COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN);
	//commSerial = new AltSoftSerial(0, 0);
}

void CommunicatorTask::__initialize() {
	commSerial->begin(COMM_BLUETOOTH_BAUD_RATE);
	watchdog->restart();
	curByteIdx = 0;
}

void CommunicatorTask::__run() {
	if (!hasTimedOut) {
		if (receiveChars()) {
			fetchMessage(recvMessage);
			__available = true;
		}

		// resets watchdog timer used for checking if connection timed out
		watchdog->restart();
	}
}

void CommunicatorTask::__onTimedOut() {
	hasTimedOut = true;
}

Message CommunicatorTask::getMessage() {
	bool messageAvailable = __available;
	__available = false;
	return messageAvailable ? recvMessage : Message::NoNewMessage;
}

bool CommunicatorTask::receiveChars() {
	bool msgReceived = false;

	if (commSerial->available() >= COMM_MESSAGE_SIZE) {
		commSerial->readBytes(static_cast<byte*>(recvBuffer), COMM_MESSAGE_SIZE);
		msgReceived = true;
	}
	return msgReceived;
}

void CommunicatorTask::fetchMessage(Message& dest) {

	//bool valid = Message::isValid(recvBuffer);
	//// parses message from buffer string
	//if (valid) Message::fromString(recvBuffer, dest);
	//// clears buffer string
	//recvBuffer = "";

	Message::fromBytes(recvBuffer, dest);
}

int CommunicatorTask::sendMessage(const Message& message) {
	return print(*commSerial, message);
}

CommunicatorTask::~CommunicatorTask() {
	delete commSerial;
}

