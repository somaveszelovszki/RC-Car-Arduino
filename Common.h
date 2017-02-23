#ifndef Common_H
#define Common_H

#include <Arduino.h>

class Common {
public:
	class PINS {
	public:
		static const int BLUETOOTH_RX = 10;
		static const int BLUETOOTH_TX = 11;
	};

	static size_t write(Print* printer, String str);

};

#endif
