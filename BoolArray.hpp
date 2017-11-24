#ifndef RC_CAR__BOOL_ARRAY__HPP
#define RC_CAR__BOOL_ARRAY__HPP

#include "Common.hpp"

namespace rc_car {

	/** @brief Mask for getting and setting element at given position in a byte container.
	*/
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

	/** @brief Array for storing binary values.
	Values are stored in a byte array (8 binary values in every byte container) to reduce memory need.

	@tparam N Number of binary values stored in the array.
	*/
	template <int N>
	class BoolArray {
	private:
		/** @brief Byte array containing the binary values.
		*/
		byte data[(N + 7) / 8];

	public:

		/** @brief Gets binary value at given position.

		@param pos The position of the value to get.
		@returns The binary value of the element.
		*/
		bool get(int pos) const {
			return data[pos / 8] & mask[pos % 8];
		}

		/** @brief Sets element at given position to the given binary value.

		@param pos The position of the value to set.
		@param value The binary value to set.
		*/
		void set(int pos, bool value) {
			byte container = mask[pos % 8];
			if (value)
				data[pos / 8] |= mask[pos % 8];
			else
				data[pos / 8] &= ~mask[pos % 8];
		}

		/** @brief Performs test-and-set: sets element at given position to given value, and returns its previous value.

		@param pos The position of the value to set.
		@param valueToSet The binary value to set.
		@param value The previous binary value of the element.
		*/
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
