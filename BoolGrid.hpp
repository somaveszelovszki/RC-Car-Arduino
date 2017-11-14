#ifndef RC_CAR__BOOL_GRID__HPP
#define RC_CAR__BOOL_GRID__HPP

#include "BoolArray.hpp"

//#define __type byte
//#define __size 8
//#define __maxValue 0x000000ff
//
//#define divBitCount(bits, container_size) (((bits) + (container_size) - 1) / (container_size))
//
//namespace rc_car {
//	template <int X, int Y>
//	class BoolGrid {
//	private:
//		__type data[divBitCount(X, __size)][Y];
//
//	public:
//		bool get(int x, int y) {
//			__type container = data[x / __size][y];
//			// shifts container to the left so that checked bit will be MSB,
//			// then cuts off bits that are on the left of this bit
//			// finally shifts container all the way to the right so that the only remaining bit is the checked one
//			return static_cast<bool>((((container << (__size - (x % __size) - 1)) & __maxValue) >> (__size - 1));
//		}
//	};
//}

namespace rc_car {
	template <int X, int Y>
	class BoolGrid {
	private:
		BoolArray<Y> data[X];

	public:
		bool get(int x, int y) const {
			return data[x].get(y);
		}

		void set(int x, int y, bool value) {
			data[x].set(y, value);
		}
	};
}

#endif // RC_CAR__BOOL_GRID__HPP
