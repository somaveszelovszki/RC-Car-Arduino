#ifndef RC_CAR__BOOL_ARRAY__HPP
#define RC_CAR__BOOL_ARRAY__HPP

#include "Common.hpp"

namespace rc_car {

	static const byte mask[8] = {
		static_cast<byte>(0b00000001),
		static_cast<byte>(0b00000010),
		static_cast<byte>(0b00000100),
		static_cast<byte>(0b00001000),
		static_cast<byte>(0b00010000),
		static_cast<byte>(0b00100000),
		static_cast<byte>(0b01000000),
		static_cast<byte>(0b10000000)
	};

	template <int N>
	class BoolArray {
	private:
		byte data[(N + 7) / 8];

	public:

		bool get(int pos) const {
			return data[pos / 8] & mask[pos % 8];
		}

		void set(int pos, bool value) {
			byte container = mask[pos % 8];
			if (value)
				data[pos / 8] |= mask[pos % 8];
			else
				data[pos / 8] &= ~mask[pos % 8];
		}

		bool testAndSet(int pos, bool valueToSet = true) {
			noInterrupts();
			bool res = get(pos);
			set(pos, valueToSet);
			interrupts();
			return res;
		}
	};
}

#endif // RC_CAR__BOOL_ARRAY__HPP
