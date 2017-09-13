#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Common.hpp"

// TODO make it template so that stores value does not have to be string

/**
Command class contains data about a received command.
*/

class Command {

public:

	enum CODE {
		NoNewCommandCode = 0,
		Speed = 1,           // [0 100] contains direction as well (>50 means FORWARD)
		SteeringAngle = 2,  // [-100 100] positive means steering to the right
		DriveMode = 3		// values from Common::DriveMode enum
	};

	static const Command NoNewCommand;

private:
	CODE code;
	String value;

public:

	Command() {}

	Command(CODE code, const String& value);

	Command operator=(const Command& other);

	Command(const Command& other) {
		*this = other;
	}

	CODE getCode() const {
		return code;
	}

	void setCode(CODE _code) {
		code = _code;
	}

	String getStringValue() const {
		return value;
	}

	void setStringValue(const String& _value) {
		value = _value;
	}

	int getIntValue() const {
		return value.toInt();
	}

	void setIntValue(int _value) {
		value = String(_value);
	}

	double getDoubleValue() const {
		return value.toDouble();
	}

	void setDoubleValue(double _value) {
		value = String(_value);
	}

	float getFloatValue() const {
		return value.toFloat();
	}

	void setFloatValue(float _value) {
		value = String(_value);
	}

	Common::DriveMode getDriveModeValue() const {
		return static_cast<Common::DriveMode>(value.toInt());
	}

	static bool isValid(char c);

	static bool isValid(const String& cmdStr);

	static void fromString(const String& cmdStr, Command& dest);
	String toString() const;
};

#endif	// COMMAND_HPP

