#include "Event.h"

Event::Event(WHAT what) {
	this->what = what;
}

Event::WHAT Event::getWhat() {
	return what;
}
