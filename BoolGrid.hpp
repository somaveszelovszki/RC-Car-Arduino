#ifndef RC_CAR__BOOL_GRID__HPP
#define RC_CAR__BOOL_GRID__HPP

#include "BoolArray.hpp"

namespace rc_car {
	/** @brief Grid (2-dimensional array) of binary values.
	Values are stored in byte arrays (8 binary values in every byte container) to reduce memory need.

	@tparam X Number of binary values stored in the X axis.
	@tparam Y Number of binary values stored in the Y axis.
	*/
	template <int X, int Y>
	class BoolGrid {
	private:
		/** @brief Array of byte arrays containing the binary values.
		*/
		BoolArray<Y> data[X];

	public:
		/** @brief Gets binary value at given X,Y coordinate.

		@param x The X coordiante of the value to get.
		@param y The Y coordiante of the value to get.
		@returns The binary value of the element.
		*/
		bool get(int x, int y) const {
			return data[x].get(y);
		}

		/** @brief Sets element at given X,Y coordinate to the given binary value.

		@param x The X coordiante of the value to get.
		@param y The Y coordiante of the value to get.
		@param value The binary value to set.
		*/
		void set(int x, int y, bool value) {
			data[x].set(y, value);
		}

		/** @brief Performs test-and-set: sets element at given X,Y coordinate to given value, and returns its previous value.

		@param x The X coordiante of the value to get.
		@param y The Y coordiante of the value to get.
		@param valueToSet The binary value to set.
		@param value The previous binary value of the element.
		*/
		bool testAndSet(int x, int y, bool valueToSet = true) {
			return data[x].testAndSet(y, valueToSet);
		}
	};
}

#endif // RC_CAR__BOOL_GRID__HPP
