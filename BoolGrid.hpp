#ifndef RC_CAR__BOOL_GRID__HPP
#define RC_CAR__BOOL_GRID__HPP

#include "BoolArray.hpp"

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
