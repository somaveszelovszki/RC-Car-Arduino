#include "Message.hpp"

using namespace rc_car;

const Message Message::NoNewMessage(Message::CODE::NoNewMessageCode, 0);

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

Message Message::operator=(const Message& other) {
	setCode(other.code);
	setValue(other.value);
	return *this;
}

//bool Message::isValid(char c) {
//	static const char permittedChars[] = {
//		' ',
//		'_',
//		'-'
//	};
//	static const int permittedCharsNum = 3;
//
//	return (c >= '0' && c <= '9')
//		|| c == MESSAGE_SEPARATOR_CHAR || c == MESSAGE_END_CHAR
//		|| (c >= 'a' && c <= 'b')
//		|| (c >= 'A' && c <= 'Z')
//		|| Common::contains(permittedChars, permittedCharsNum, c);
//}
//
//bool Message::isValid(const String& msgStr) {
//	bool valid = msgStr != NULL && msgStr.length() > 0;
//
//	for (int i = 0; valid && i < msgStr.length(); ++i)
//		valid = isValid(msgStr.charAt(i));
//
//	return valid;
//}
//
///**
// * Parses Message from String object.
// * e.g. incoming String is "1:10"
// * output will be: 
// *    Message { code=1 and value=10 }
// */
//void Message::fromString(const String& msgStr, Message& dest) {
//
//	// finds separator in string (separates key from value)
//	int sepIdx = msgStr.indexOf(MESSAGE_SEPARATOR_CHAR);
//
//	// code is before separator
//	dest.code = static_cast<Message::CODE>(msgStr.substring(0, sepIdx).toInt());
//
//	// value is after separator
//	dest.value = msgStr.substring(sepIdx + 1, msgStr.length() - 1);
//}

ByteArray<COMM_MESSAGE_SIZE> Message::toBytes() const {
	return codeToByte(code) + value;
}

void Message::fromBytes(const ByteArray<COMM_MESSAGE_SIZE>& bytes, Message& dest) {
	dest.setCode(byteToCode(bytes[0]));
	dest.setValue(&bytes[1]);
}

/**
 * Generates String from Message object.
 * e.g. code=1 and value=10
 * output will be "1:10;"
 */
String Message::toString() const {
	return String(code) + ": " + value.toString();
}

int print(Print & printer, const Message & msg) {
	return print(printer, msg.toBytes());
}
