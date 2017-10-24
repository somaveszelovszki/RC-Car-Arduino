#include "Message.hpp"

using namespace rc_car;

const Message Message::ACK(Message::CODE::ACK_, 0);

Message::Message(CODE _code, const ByteArray<4>& _value) {
	setCode(_code);
	setValue(_value);
}

Message::Message(CODE _code, int _value) {
	setCode(_code);
	setValue(_value);
}

Message::Message(CODE _code, float _value) {
	setCode(_code);
	setValue(_value);
}

ByteArray<COMM_MESSAGE_SIZE> Message::toBytes() const {
	return codeToByte(code) + value;
}

void Message::fromBytes(const ByteArray<COMM_MESSAGE_SIZE>& bytes, Message& dest) {
	dest.setCode(byteToCode(bytes[0]));
	dest.setValue(&bytes[1]);
}

#if __DEBUG
/**
* Generates String from Message object.
* e.g. code=1 and value=10
* output will be "1:10;"
*/
String Message::toString() const {
	return String(code) + ": " + value.toString();
}
#endif // __DEBUG
