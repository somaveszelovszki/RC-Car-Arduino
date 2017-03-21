#ifndef CONNECTIONTIMEDOUTEVENT_H
#define CONNECTIONTIMEDOUTEVENT_H


#include "Event.h"
#include "Command.h"

class ConnectionTimedOutEvent : public Event {

public:
	ConnectionTimedOutEvent();
};

#endif
