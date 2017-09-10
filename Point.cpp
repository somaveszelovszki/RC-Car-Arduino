#include "Point.hpp"

template <typename T>
float Point<T>::getSteeringAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir) {
	float angle;

	switch (dir) {
	case Common::SteeringDir::LEFT:
		if (other.X > origo.X)
			angle = other.Y >= origo.Y ?
			static_cast<float>(atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))))
			: static_cast<float>(2 * M_PI + atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))));
		else if (other.X < origo.X)
			angle = static_cast<float>(M_PI + atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))));
		else
			angle = other.Y > origo.Y ? static_cast<float>(M_PI_2) : 3 * static_cast<float>(M_PI_2);
	case Common::SteeringDir::RIGHT:
		angle = -1 * getSteeringAngle(origo, Point<T>(2 * origo.X - other.X, other.Y), Common::SteeringDir::LEFT);
	}
	return angle;
}

#if(__DEBUG)
template<typename T>
void Point<T>::print(bool newLine = true) const {
	Serial.print("(");
	Serial.print(X);
	Serial.print(", ");
	Serial.print(Y);
	Serial.print(")");
	if (newLine) Serial.println();
}
#endif // __DEBUG
