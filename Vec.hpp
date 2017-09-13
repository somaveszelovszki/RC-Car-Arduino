#ifndef VEC_HPP
#define VEC_HPP

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

#endif // VEC_HPP