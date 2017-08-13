#include "Point.hpp"

template <typename T>
inline T Point<T>::distanceFrom(Point<T> other) {
	return Common::pythagoreanHypotenuse(X - other.X, Y - other.Y);
}

template<typename T>
double Point<T>::getAngle(Point<T> other) {
	return other.X > X ? atan((other.Y - Y) / (other.X - X))
		: other.X < X ? PI + atan((other.Y - Y) / (other.X - X))
		: other.Y - Y > 0 ? HALF_PI : -HALF_PI;
}
