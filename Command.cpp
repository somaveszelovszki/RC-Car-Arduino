#include "Command.hpp"

const Command Command::NoNewCommand(Command::CODE::NoNewCommandCode, "");

Command::Command(Command::CODE code, const String& value) {
  this->code = code;
  this->value = value;
}

Command Command::operator=(const Command& other) {
	this->code = other.code;
	this->value = other.value;

	return *this;
}

bool Command::isValid(char c) {
	static const char permittedChars[] = {
		' ',
		'_',
		'-'
	};
	static const int permittedCharsNum = 3;

	return (c >= '0' && c <= '9')
		|| c == COMMAND_SEPARATOR_CHAR || c == COMMAND_END_CHAR
		|| (c >= 'a' && c <= 'b')
		|| (c >= 'A' && c <= 'Z')
		|| Common::contains(permittedChars, permittedCharsNum, c);
}

bool Command::isValid(const String& cmdStr) {
	bool valid = cmdStr != NULL && cmdStr.length() > 0;

	for (int i = 0; valid && i < cmdStr.length(); ++i)
		valid = isValid(cmdStr.charAt(i));

	return valid;
}

/**
 * Parses Command from String object.
 * e.g. incoming String is "1:10"
 * output will be: 
 *    Command { code=1 and value=10 }
 */
void Command::fromString(const String& cmdStr, Command& dest) {

	// finds separator in string (separates key from value)
	int sepIdx = cmdStr.indexOf(COMMAND_SEPARATOR_CHAR);

	// code is before separator
	dest.code = static_cast<Command::CODE>(cmdStr.substring(0, sepIdx).toInt());

	// value is after separator
	dest.value = cmdStr.substring(sepIdx + 1, cmdStr.length() - 1);
}

/**
 * Generates String from Command object.
 * e.g. code=1 and value=10
 * output will be "1:10;"
 */
String Command::toString() const {
	return String(code) + COMMAND_SEPARATOR_CHAR + value + COMMAND_END_CHAR;
}

