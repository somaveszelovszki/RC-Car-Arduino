#include "Command.hpp"

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

bool Command::isValid() const {
	String commandString = this->toString();

	if (commandString == NULL || commandString.length() == 0) return false;

	for (int i = 0; i < commandString.length(); ++i) {
		if (!isValid(commandString.charAt(i)))
			return false;
	}

	return true;
}

bool Command::isValid(char c) const {
	if (c >= '0' && c <= '9') return true;

	if (c == COMMAND_SEPARATOR_CHAR || c == COMMAND_END_CHAR) return true;

	if (c >= 'a' && c <= 'b') return true;

	if (c >= 'A' && c <= 'Z') return true;

	int charsNum = 3;
	char chars[] = {
		' ',
		'_',
		'-'
	};

	if (Common::contains(chars, charsNum, c)) return true;

	return false;
}

/**
 * Parses Command from String object.
 * e.g. incoming String is "1:10"
 * output will be: 
 *    Command { code=1 and value=10 }
 */
void Command::fromString(String commandString, Command& dest) {

	// finds separator in string (separates key from value)
	int separatorIndex = commandString.indexOf(COMMAND_SEPARATOR_CHAR);

	// code is before separator
	dest.code = static_cast<Command::CODE>(commandString.substring(0, separatorIndex).toInt());

	// value is after separator
	dest.value = commandString.substring(separatorIndex + 1, commandString.length() - 1);
}

/**
 * Generates String from Command object.
 * e.g. code=1 and value=10
 * output will be "1:10;"
 */
String Command::toString() const {
	String codeStr(code);

	return codeStr + COMMAND_SEPARATOR_CHAR + value + COMMAND_END_CHAR;
}

