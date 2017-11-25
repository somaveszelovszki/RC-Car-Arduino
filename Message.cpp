#include "Message.hpp"

using namespace rc_car;

const ByteArray<COMM_MSG_SEPARATOR_LENGTH> Message::SEPARATOR(COMM_MSG_SEPARATOR);

const ByteArray<COMM_MSG_DATA_LENGTH> Message::BOOL_value_TRUE(static_cast<int32_t>(1));
const ByteArray<COMM_MSG_DATA_LENGTH> Message::BOOL_value_FALSE(static_cast<int32_t>(0));

const Message Message::ACK(Message::CODE::ACK_, static_cast<int32_t>(0));

ByteArray<COMM_MSG_LENGTH> Message::toBytes() const {
    return SEPARATOR + getCodeAsByte() + data;
}

void Message::fromBytes(const ByteArray<COMM_MSG_LENGTH>& bytes, Message& result) {
    // skips SEPARATOR
    result.setCode(bytes[COMM_MSG_SEPARATOR_LENGTH]);
    result.setData(&bytes[COMM_MSG_SEPARATOR_LENGTH + COMM_MSG_CODE_LENGTH]);
}

#if __DEBUG
String Message::toString() const {
    return String(code) + ": " + data.toString();
}
#endif // __DEBUG
