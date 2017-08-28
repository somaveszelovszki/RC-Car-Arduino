#include "Point.hpp"

template <typename T>
double Point<T>::getSteeringAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir = Common::SteeringDir::LEFT) {
	switch (dir) {
	case Common::SteeringDir::LEFT:
		if (other.X > origo.X)
			return other.Y >= origo.Y ?
			atan((other.Y - origo.Y) / (other.X - origo.X))
			: 2 * M_PI + atan((other.Y - origo.Y) / (other.X - origo.X));
		else if (other.X < origo.X)
			return M_PI + atan((other.Y - origo.Y) / (other.X - origo.X));
		else
			return other.Y > origo.Y ? M_PI_2 : 3 * M_PI_2;
	case Common::SteeringDir::RIGHT:
		return (-1) * getSteeringAngle(origo, Point<T>(origo.X - (other.X - origo.X), other.Y), Common::SteeringDir::LEFT);
	default:
		throw invalid_argument("'dir' has to be either LEFT or RIGHT");
	}
}
