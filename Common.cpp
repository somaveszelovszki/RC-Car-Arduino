#include "Common.h"

size_t Common::write(Print* printer, String str) {

	size_t numBytes = 0;

	for (size_t i = 0; i < str.length(); i++) {
		numBytes += printer->print(str.charAt(i));
	}

	return numBytes;
}