#ifndef COMMAND_H
#define COMMAND_H

#include "Common.h"

/**
   Command class contains data about a received command.
*/
class Command {

public:
	enum CODE {
		Speed = 1,           // [0 100] contains direction as well (>50 means FORWARD)
		SteeringAngle = 2,  // [-100 100] positive means steering to the right
		ServoRecalibrate = 3
	};


private:
	CODE code;
	String value;

public:

	static const char END_CHAR = ';';
	static const char SEPARATOR_CHAR = ':';

	Command(CODE code, String value);

	CODE getCode();

	String getValue();

	int getValueAsInt();

	static Command* fromString(String commandString);
	String toString();
};

#endif

