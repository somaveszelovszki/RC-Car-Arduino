#include "Event.hpp"

Event::Event(WHAT what) {
	this->what = what;
}

Event::WHAT Event::getWhat() const {
	return what;
}
