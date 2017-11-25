#include "CommunicatorTask.hpp"

using namespace rc_car;

void CommunicatorTask::initialize() {

    pinMode(COMM_RX_PIN, INPUT);
    pinMode(COMM_TX_PIN, OUTPUT);

    for (int i = 0; i < TASK_COUNT; ++i) {
        __recvAvailable[i] = false;
        __sendAvailable[i] = false;
    }

    Serial.begin(COMM_BLUETOOTH_BAUD_RATE);
    while (!Serial) {}

    recvByteIdx = 0;
}

void CommunicatorTask::run() {
    if (receiveChars()) {
        fetchMessage();
        for (int i = 0; i < TASK_COUNT; ++i)
            __recvAvailable[i] = true;
    }

    for (int i = 0; i < TASK_COUNT; ++i)
        if (__sendAvailable[i]) {
            __sendAvailable[i] = false;
            sendMessage(i);
        }
}

const Message& CommunicatorTask::getReceivedMessage(int taskId) {
    __recvAvailable[taskId] = false;
    return recvMsg;
}

bool CommunicatorTask::setMessageToSend(const Message& msg, int taskId) {
    bool noHighPrioInBuff = !__sendAvailable[taskId] || sendMsgs[taskId].getPriority() < Message::HIGH_PRIO;
    if (noHighPrioInBuff) {
        sendMsgs[taskId] = msg;
        __sendAvailable[taskId] = true;
    }

    return noHighPrioInBuff;
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
    }

    return msgReceived;
}

void CommunicatorTask::fetchMessage() {
    Message::fromBytes(recvBuffer, recvMsg);
}

int CommunicatorTask::sendMessage(int taskId) {
    return Serial.write(static_cast<const byte*>(sendMsgs[taskId].toBytes()), COMM_MSG_LENGTH);
}