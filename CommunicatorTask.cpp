#include "CommunicatorTask.hpp"

using namespace rc_car;

CommunicatorTask::CommunicatorTask() : PeriodicTask(TASK_PERIOD_TIME_COMMUNICATOR, TASK_WATCHDOG_TIMEOUT_COMMUNICATOR) {
	//commSerial = new SoftwareSerial(COMM_BLUETOOTH_RX_PIN, COMM_BLUETOOTH_TX_PIN);
	//commSerial = new AltSoftSerial(0, 0);
	pinMode(COMM_RX_PIN, INPUT);
	pinMode(COMM_TX_PIN, OUTPUT);

	for (int i = 0; i < TASK_COUNT; ++i) {
		__recvAvailable[i] = NOT_AVAILABLE;
		__sendAvailable[i] = NOT_AVAILABLE;
	}
}

void CommunicatorTask::initialize() {
	Serial.begin(COMM_BLUETOOTH_BAUD_RATE);
	while (!Serial) {}

	recvByteIdx = 0;
}

void CommunicatorTask::run() {
	if (receiveChars()) {
		fetchMessage();
		for (int i = 0; i < TASK_COUNT; ++i)
			__recvAvailable[i] = AVAILABLE_LOW_PRIO;
	}

	for (int i = 0; i < TASK_COUNT; ++i)
		if (__sendAvailable[i]) {
			__sendAvailable[i] = NOT_AVAILABLE;
			sendMessage(i);
		}
}

void CommunicatorTask::onTimedOut() {
	restartTimeoutCheck();
}

void CommunicatorTask::getReceivedMessage(Message& msg, int taskId) {
	if (__recvAvailable[taskId]) {
		__recvAvailable[taskId] = NOT_AVAILABLE;
		msg = recvMsg;
	}
}

void CommunicatorTask::setMessageToSend(const Message& msg, int taskId, MsgAvailability availability = AVAILABLE_LOW_PRIO) {
	sendMsgs[taskId] = msg;
	__sendAvailable[taskId] = availability;
}

bool CommunicatorTask::receiveChars() {
	int availableBytesNum = Serial.available();

	bool msgReceived = false;
	if (availableBytesNum) {
		int bytesNum = min(availableBytesNum, COMM_MSG_LENGTH - recvByteIdx);

		for (int i = 0; i < bytesNum; ++i) {
			byte b = static_cast<byte>(Serial.read());

			switch (recvState) {
			case READ_SEP:
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
					recvState = READ_SEP;
					msgReceived = true;
				}
				break;
			}
		}
	}

	return msgReceived;
}

void CommunicatorTask::fetchMessage() {
	Message::fromBytes(recvBuffer, recvMsg);
}

int CommunicatorTask::sendMessage(int taskId) {
	return Serial.write(static_cast<const byte*>(sendMsgs[taskId].toBytes()), COMM_MSG_LENGTH);
}