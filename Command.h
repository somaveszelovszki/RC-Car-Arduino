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
		ServoRecalibrate = 3,
		DriveMode = 4,		// values from DriveController::MODE enum
	};


private:
	CODE code;
	String value;

public:

	static const char END_CHAR = ';';
	static const char SEPARATOR_CHAR = ':';

	Command(CODE code, const String& value);

	Command(const Command& other);

	Command operator=(const Command& other);

	CODE getCode() const;

	String getValue() const;

	int getValueAsInt() const;

	static Command* fromString(String commandString);
	String toString() const;
};

#endif

