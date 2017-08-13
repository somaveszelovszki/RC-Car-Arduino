#ifndef CONNECTIONTIMEDOUTEVENT_HPP
#define CONNECTIONTIMEDOUTEVENT_HPP


#include "Event.hpp"
#include "Command.hpp"

class ConnectionTimedOutEvent : public Event {

public:
	ConnectionTimedOutEvent();
};

#endif	// CONNECTIONTIMEDOUTEVENT_HPP
