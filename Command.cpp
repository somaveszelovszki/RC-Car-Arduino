#include "Command.h"

Command::Command(Command::CODE code, String value) {
  this->code = code;
  this->value = value;
}

Command::CODE Command::getCode() {
	return code;
}

String Command::getValue() {
	return value;
}

int Command::getValueAsInt() {
	return value.toInt();
}

/**
 * Parses Command from String object.
 * e.g. incoming String is "1:10"
 * output will be: 
 *    Command { code=1 and value=10 }
 */
Command* Command::fromString(String commandString) {

	Serial.println(commandString);

  // finds separator in string (separates key from value)
  int separatorIndex = commandString.indexOf(SEPARATOR_CHAR);

  // code is before separator
  int code = commandString.substring(0, separatorIndex).toInt();

  // value is after separator
  String value = commandString.substring(separatorIndex + 1);

  return new Command((CODE) code, value);
}

/**
 * Generates String from Command object.
 * e.g. code=1 and value=10
 * output will be "1:10;"
 */
String Command::toString() {

  String codeStr(code);

  return codeStr + SEPARATOR_CHAR + value + END_CHAR;
}

