#ifndef RC_CAR__BOOL_ARRAY__HPP
#define RC_CAR__BOOL_ARRAY__HPP

#include "Common.hpp"

namespace rc_car {
	template <int N>
	class BoolArray {
	private:
		byte data[(N + 7) / 8];

		static const byte mask[8] = {
			0b00000001,
			0b00000010,
			0b00000100,
			0b00001000,
			0b00010000,
			0b00100000,
			0b01000000,
			0b10000000
		};

	public:

		bool get(int pos) const {
			return data[pos / 8] & mask[pos % 8];
		}

		void set(int pos, bool value) {
			byte container = mask[pos % 8];
			if (value)
				data[pos / 8] |= container;
			else
				data[pos / 8] &= (container ^ container);
		}
	};
}

#endif // RC_CAR__BOOL_ARRAY__HPP
