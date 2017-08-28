#ifndef POINT_HPP
#define POINT_HPP

#include "Common.hpp"

using namespace std;

template <typename T>
class Point {
public:
	T X;
	T Y;

	Point() {}
	Point(T x, T y) : X(x), Y(y) {}

	T distanceFrom(Point<T> other) {
		return Common::pythagoreanHypotenuse(X - other.X, Y - other.Y);
	}

	static double getSteeringAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir = Common::SteeringDir::LEFT);
	/*
	Calculates steering angle of given point using this point as the origo.
	NOTE: Steering angle is always positive for LEFT, and always negative for RIGHT steering direction!
	*/
	double getSteeringAngle(const Point<T>& other, Common::SteeringDir dir = Common::SteeringDir::LEFT) {
		return getSteeringAngle(*this, other, dir);
	}
};

template <typename T>
ostream& operator << (ostream& os, const Point<T>& point) {
	os << "(" << point.X << ", " << point.Y << ")";
	return os;
}

#endif	// POINT_HPP
