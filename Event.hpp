#ifndef EVENT_HPP
#define EVENT_HPP

#include "Common.hpp"

class Event {
public:
	enum WHAT {
		CommandReceived = 1,
		ConnectionTimedOut = 2,
		DangerSensed = 3
	};

private:
	WHAT what;

public:

	Event(WHAT what);

	WHAT getWhat();
};

#endif	// EVENT_HPP
