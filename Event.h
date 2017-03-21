#ifndef EVENT_H
#define EVENT_H

#include "Common.h"

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

#endif
