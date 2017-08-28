#ifndef EVENT_HPP
#define EVENT_HPP

// NOT USED yet

#include "Common.hpp"

class Event {
public:
	enum WHAT {
		CommandReceived = 1,
		ConnectionTimedOut = 2
	};

private:
	WHAT what;

public:

	Event(WHAT what);

	WHAT getWhat();
};

#endif	// EVENT_HPP
