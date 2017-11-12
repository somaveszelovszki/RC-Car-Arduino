#include "CommunicatorTask.hpp"

using namespace rc_car;

CommunicatorTask::CommunicatorTask() : PeriodicTask(PT_PERIOD_TIME_COMMUNICATOR, PT_WATCHDOG_TIMEOUT_COMMUNICATOR) {
	//commSerial = new SoftwareSerial(COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN);
	//commSerial = new AltSoftSerial(0, 0);
	pinMode(COMM_RX_PIN, INPUT);
	pinMode(COMM_TX_PIN, OUTPUT);

	for (int i = 0; i < PT_NUM_TASKS; ++i) {
		__recvAvailable[i] = __sendAvailable[i] = false;
	}
}

void CommunicatorTask::initialize() {
	Serial.begin(COMM_BLUETOOTH_BAUD_RATE);
	while (!Serial) {}

	recvByteIdx = 0;
}

void CommunicatorTask::run() {
	if (true || !__hasTimedOut) {	// TODO remove 'true ||'

		if (receiveChars()) {
			fetchMessage();
			for (int i = 0; i < PT_NUM_TASKS; ++i)
				__recvAvailable[i] = true;
		}

		for (int i = 0; i < PT_NUM_TASKS; ++i)
			if (Common::testAndSet(&__sendAvailable[i], false))
				sendMessage(i);
	}
}

void CommunicatorTask::onTimedOut() {
	__hasTimedOut = true;
}

bool CommunicatorTask::getReceivedMessage(Message& msg, int taskId) {
	bool isNewMessageAvailable = Common::testAndSet(&__recvAvailable[taskId], false);
	if (isNewMessageAvailable)
		msg = recvMsg;
	return isNewMessageAvailable;
}

void rc_car::CommunicatorTask::setMessageToSend(const Message& msg, int taskId) {
	sendMsgs[taskId] = msg;
	__sendAvailable[taskId] = true;
}

bool CommunicatorTask::receiveChars() {
	int availableBytesNum = Serial.available();

	bool msgReceived = false;
	if (availableBytesNum) {
		int bytesNum = min(availableBytesNum, COMM_MSG_LENGTH - recvByteIdx);

		for (int i = 0; i < bytesNum; ++i) {
			byte b = static_cast<byte>(Serial.read());

			switch (recvState) {
			case READ_SEPARATOR:
				if (b == Message::SEPARATOR[recvByteIdx]) {
					recvBuffer[recvByteIdx++] = b;
					if (recvByteIdx == COMM_MSG_SEPARATOR_LENGTH)
						recvState = READ_CODE;
				} else
					recvByteIdx = 0;
				break;
			case READ_CODE:
				recvBuffer[recvByteIdx++] = b;
				if (recvByteIdx == COMM_MSG_SEPARATOR_LENGTH + COMM_MSG_CODE_LENGTH)
					recvState = READ_DATA;
				break;
			case READ_DATA:
				recvBuffer[recvByteIdx++] = b;
				if (recvByteIdx == COMM_MSG_LENGTH) {
					recvByteIdx = 0;
					recvState = READ_SEPARATOR;
					msgReceived = true;
				}
				break;
			}
		}

		//Serial.readBytes(static_cast<byte*>(recvBuffer) + recvByteIdx, bytesNum);
		//recvByteIdx += bytesNum;

		//// if SEPARATOR is not at the beginning of the byte array, shifts it
		//int sepIndex = recvBuffer.indexOf(COMM_MSG_SEPARATOR);

		//if (sepIndex > 0) {
		//	recvBuffer.shiftBytesLeft(sepIndex);
		//	recvByteIdx -= sepIndex;
		//} else if (sepIndex == -1)
		//	recvByteIdx = 0;
	}

	//return recvByteIdx == COMM_MSG_LENGTH;

	return msgReceived;
}

void CommunicatorTask::fetchMessage() {

	//bool valid = Message::isValid(recvBuffer);
	//// parses message from buffer string
	//if (valid) Message::fromString(recvBuffer, dest);
	//// clears buffer string 
	//recvBuffer = "";

	Message::fromBytes(recvBuffer, recvMsg);
//	recvByteIdx = 0;
}

int CommunicatorTask::sendMessage(int taskId) {
	return Serial.write(static_cast<const byte*>(sendMsgs[taskId].toBytes()), COMM_MSG_LENGTH);
}