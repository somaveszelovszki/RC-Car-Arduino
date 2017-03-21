#include "Command.h"

Command::Command(Command::CODE code, const String& value) {
  this->code = code;
  this->value = value;
}

Command::Command(const Command& other) {
	*this = other;
}

Command Command::operator=(const Command& other) {
	this->code = other.code;
	this->value = other.value;

	return *this;
}


Command::CODE Command::getCode() const {
	return code;
}

String Command::getValue() const {
	return value;
}

int Command::getValueAsInt() const {
	return value.toInt();
}

/**
 * Parses Command from String object.
 * e.g. incoming String is "1:10"
 * output will be: 
 *    Command { code=1 and value=10 }
 */
Command* Command::fromString(String commandString) {

	// finds separator in string (separates key from value)
	int separatorIndex = commandString.indexOf(SEPARATOR_CHAR);

	// code is before separator
	int code = commandString.substring(0, separatorIndex).toInt();

	// value is after separator
	String value = commandString.substring(separatorIndex + 1, commandString.length() - 1);

	return new Command((CODE) code, value);
}

/**
 * Generates String from Command object.
 * e.g. code=1 and value=10
 * output will be "1:10;"
 */
String Command::toString() const {
	String codeStr(code);

	return codeStr + SEPARATOR_CHAR + value + END_CHAR;
}

