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

	T distanceFrom(Point<T> other);

	/*
		Calculates angle of given point using this point as the origo.
	*/
	double getAngle(Point<T> other);
};

#endif	// POINT_HPP
