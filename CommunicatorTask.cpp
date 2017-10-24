#include "CommunicatorTask.hpp"

using namespace rc_car;

CommunicatorTask::CommunicatorTask() : PeriodicTask(PT_PERIOD_TIME_COMMUNICATOR, PT_WATCHDOG_TIMEOUT_COMMUNICATOR) {
	//commSerial = new SoftwareSerial(COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN);
	//commSerial = new AltSoftSerial(0, 0);
	pinMode(COMM_EN_PIN, OUTPUT);
	pinMode(COMM_RX_PIN, INPUT);
	pinMode(COMM_TX_PIN, OUTPUT);
}

void CommunicatorTask::__initialize() {
	Serial.begin(COMM_BLUETOOTH_BAUD_RATE);
	while (!Serial) {}
	digitalWrite(COMM_EN_PIN, COMM_EN_SIGNAL_LEVEL);	// enables communication

	curByteIdx = 0;
}

void CommunicatorTask::__run(void *unused) {
	if (!hasTimedOut) {
		if (receiveChars()) {
			fetchMessage();
			__recvAvailable = true;
		}

		if (__sendAvailable) {
			sendMessage();
			__sendAvailable = false;
		}
	}
}

void CommunicatorTask::__onTimedOut() {
	hasTimedOut = true;
}

bool CommunicatorTask::getReceivedMessage(Message& msg) {
	bool isNewMessageAvailable = Common::testAndSet(&__recvAvailable, false);
	if (isNewMessageAvailable)
		msg = recvMsg;
	return isNewMessageAvailable;
}

void rc_car::CommunicatorTask::setMessageToSend(const Message & msg) {
	sendMsg = msg;
	__sendAvailable = true;
}

bool CommunicatorTask::receiveChars() {
	bool msgReceived = false;
	if (Serial.available() >= COMM_MESSAGE_SIZE) {
		Serial.readBytes(static_cast<byte*>(recvBuffer), COMM_MESSAGE_SIZE);
		msgReceived = true;
	}

	return msgReceived;
}

void CommunicatorTask::fetchMessage() {

	//bool valid = Message::isValid(recvBuffer);
	//// parses message from buffer string
	//if (valid) Message::fromString(recvBuffer, dest);
	//// clears buffer string
	//recvBuffer = "";

	Message::fromBytes(recvBuffer, recvMsg);
}

int CommunicatorTask::sendMessage() {
	//Common::debug_print("Sending: ");
	//Common::debug_println(sendMsg.toBytes().toString());
	//TODO sending not working properly!
	return Serial.write(static_cast<const byte*>(sendMsg.toBytes()), COMM_MESSAGE_SIZE);
}

