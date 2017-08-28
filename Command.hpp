#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Common.hpp"

/**
   Command class contains data about a received command.
*/
class Command {

public:
	enum CODE {
		Speed = 1,           // [0 100] contains direction as well (>50 means FORWARD)
		SteeringAngle = 2,  // [-100 100] positive means steering to the right
		ServoRecalibrate = 3,
		DriveMode = 4		// values from DriveThread::MODE enum
	};


private:
	CODE code;
	String value;

public:

	Command() {}

	Command(CODE code, const String& value);

	Command(const Command& other);

	Command operator=(const Command& other);

	CODE getCode() const;

	String getValue() const;

	int getValueAsInt() const;

	bool isValid() const;

	static void fromString(String commandString, Command& dest);
	String toString() const;

	bool isValid(char c) const;
};

#endif	// COMMAND_HPP

