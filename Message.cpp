#include "Message.hpp"

using namespace rc_car;

const Message::Code Message::CODES[Message::CODE::NUM_CODES] = {
    Code(CODE::ACK_,            0b00000000                  ),
    Code(CODE::Speed,           0b00000001                  ),
    Code(CODE::SteeringAngle,   0b00000010                  ),
    Code(CODE::DriveMode,       0b00000011                  ),
    Code(CODE::RelEnvEn,        0b00000111                  ),
    Code(CODE::RelEnvPoint,     0b00001000,     0b11111000  ),
    Code(CODE::EnvGridEn,       0b00000100                  ),
    Code(CODE::EnvGrid,         0b01000000,     0b11000000  )
};

Message::CODE Message::Code::apply(byte code) {
    CODE result;
    for (int i = 0; result == NUM_CODES && i < CODE::NUM_CODES; ++i) {
        Code codeObj = CODES[i];
        if (codeObj.codeByte == (code & codeObj.matchPattern))
            result = codeObj.codeIdx;
    }

    return result;
}

const ByteArray<COMM_MSG_SEPARATOR_LENGTH> Message::SEPARATOR(COMM_MSG_SEPARATOR);

const ByteArray<COMM_MSG_DATA_LENGTH> Message::BOOL_TRUE(static_cast<int32_t>(1));
const ByteArray<COMM_MSG_DATA_LENGTH> Message::BOOL_FALSE(static_cast<int32_t>(0));

const Message Message::ACK(Message::CODE::ACK_, static_cast<int32_t>(0));

ByteArray<COMM_MSG_LENGTH> Message::toBytes() const {
    return SEPARATOR + codeByte + data;
}

void Message::fromBytes(const ByteArray<COMM_MSG_LENGTH>& bytes, Message& result) {
    // skips SEPARATOR
    result.codeByte = bytes[COMM_MSG_SEPARATOR_LENGTH];
    result.code = CODES[Code::apply(result.codeByte)];
    result.setData(&bytes[COMM_MSG_SEPARATOR_LENGTH + COMM_MSG_CODE_LENGTH]);
}

#if _DEBUG
String Message::toString() const {
    return String(codeByte) + ": " + data.toString();
}
#endif // _DEBUG
