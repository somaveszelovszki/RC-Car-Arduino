#ifndef POINT_HPP
#define POINT_HPP

#include "Common.hpp"

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

	static float getSteeringAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir);

	/*
	Calculates steering angle of given point using this point as the origo.
	NOTE: Steering angle is always positive for LEFT, and always negative for RIGHT steering direction!
	*/
	float getSteeringAngle(const Point<T>& other, Common::SteeringDir dir) {
		return getSteeringAngle(*this, other, dir);
	}

#if(__DEBUG)
	void print(bool newLine = true) const;
#endif	// __DEBUG
};

#endif	// POINT_HPP
