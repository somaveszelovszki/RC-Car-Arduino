#include "Message.hpp"

using namespace rc_car;

const ByteArray<COMM_MSG_SEPARATOR_LENGTH> Message::SEPARATOR(COMM_MSG_SEPARATOR);

const Message Message::ACK(Message::CODE::ACK_, static_cast<int32_t>(0));

Message::Message(CODE _code, const ByteArray<4>& _value) {
	setCode(_code);
	setData(_value);
}

Message::Message(CODE _code, int32_t _value) {
	setCode(_code);
	setData(_value);
}

Message::Message(CODE _code, float _value) {
	setCode(_code);
	setData(_value);
}

ByteArray<COMM_MSG_LENGTH> Message::toBytes() const {
	return SEPARATOR + codeToByte(code) + data;
}

void Message::fromBytes(const ByteArray<COMM_MSG_LENGTH>& bytes, Message& dest) {
	// skips SEPARATOR
	dest.setCode(byteToCode(bytes[COMM_MSG_SEPARATOR_LENGTH]));
	dest.setData(&bytes[COMM_MSG_SEPARATOR_LENGTH + COMM_MSG_CODE_LENGTH]);
}

#if __DEBUG
/**
* Generates String from Message object.
* e.g. code=1 and value=10
* output will be "1:10;"
*/
String Message::toString() const {
	return String(code) + ": " + data.toString();
}
#endif // __DEBUG
