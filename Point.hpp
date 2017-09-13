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

	Point<float>& operator=(const Point<float>& other);

	Point(const Point<float>& other) {
		*this = other;
	}

	Point<T> operator+(const Point<T>& other) const;
	Point<T> operator-(const Point<T>& other) const;
	Point<T>& operator+=(const Point<T>& other);
	Point<T>& operator-=(const Point<T>& other);

	Point<T> operator*(const T& c) const;
	Point<T> operator/(const T& c) const;
	Point<T>& operator*=(const T& c);
	Point<T>& operator/=(const T& c);

	friend Point<T> operator*(const T& c, const Point<T>& p);

	T distanceFrom(Point<T> other);

	T length();

	static float getSteeringAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir);

	/*
	Calculates steering angle of given point using this point as the origo.
	NOTE: Steering angle is always positive for LEFT, and always negative for RIGHT steering direction!
	*/
	float getSteeringAngle(const Point<T>& other, Common::SteeringDir dir) {
		return getSteeringAngle(*this, other, dir);
	}

	static Point<T> average(const Point<T>& p1, const Point<T>& p2);

#if(__DEBUG)
	void print(bool newLine = true) const;
#endif	// __DEBUG
};

#endif	// POINT_HPP
