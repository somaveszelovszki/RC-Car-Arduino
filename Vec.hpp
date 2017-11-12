#ifndef RC_CAR__VEC__HPP
#define RC_CAR__VEC__HPP

namespace rc_car {
	/** @brief Template vector implementation with static maximum length.
	TODO remove if not needed!
	*/
	template <typename T, int maxLength>
	class Vec {
	private:
		T data[maxLength];

	public:
		int length = 0;

		T& operator [] (int pos) {
			return data[pos];
		}

		const T& operator [] (int pos) const {
			return data[pos];
		}
	};
}

#endif // RC_CAR__VEC__HPP