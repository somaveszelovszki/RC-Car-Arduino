#ifndef POINT_HPP
#define POINT_HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Template class for storing 2-dimensional points.
	*/
	template <typename T>
	class Point {
	public:
		T X;
		T Y;

		static const Point<T> ORIGO;

		Point() {}
		Point(T x, T y) : X(x), Y(y) {}

		Point<T>& operator=(const Point<T>& other);

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

		friend Point<T> operator*(const T& c, const Point<T>& p) {
			return p * c;
		}

		T distanceFrom(Point<T> other) const;

		T length() const;

		static float getAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir);

		/*
		Calculates steering angle of given point using this point as the origo.
		NOTE: Steering angle is always positive for LEFT, and always negative for RIGHT steering direction!
		*/
		float getAngle(const Point<T>& other, Common::SteeringDir dir) const {
			return getAngle(*this, other, dir);
		}

		static Point<T> average(const Point<T>& p1, const Point<T>& p2);

#if(__DEBUG)
		void print(bool newLine = true) const;
#endif	// __DEBUG
	};



	template <typename T>
	const Point<T> Point<T>::ORIGO(static_cast<T>(0), static_cast<T>(0));

	template<typename T>
	Point<T>& Point<T>::operator=(const Point<T>& other) {
		X = other.X;
		Y = other.Y;
	}

	template<typename T>
	Point<T> Point<T>::operator+(const Point<T>& other) const {
		return Point<T>(X + other.X, Y + other.Y);
	}

	template<typename T>
	Point<T> Point<T>::operator-(const Point<T>& other) const {
		return Point<T>(X - other.X, Y - other.Y);
	}

	template<typename T>
	Point<T>& Point<T>::operator+=(const Point<T>& other) {
		X += other.X;
		Y += other.Y;
		return *this;
	}

	template<typename T>
	Point<T>& Point<T>::operator-=(const Point<T>& other) {
		X -= other.X;
		Y -= other.Y;
		return *this;
	}

	template<typename T>
	Point<T> Point<T>::operator*(const T & c) const {
		return Point<T>(X * c, Y * c);
	}

	template<typename T>
	Point<T> Point<T>::operator/(const T & c) const {
		return Point<T>(X / c, Y / c);
	}

	template<typename T>
	Point<T>& Point<T>::operator*=(const T & c) {
		X *= c;
		Y *= c;
		return *this;
	}

	template<typename T>
	Point<T>& Point<T>::operator/=(const T & c) {
		X /= c;
		Y /= c;
		return *this;
	}

	template<typename T>
	T Point<T>::distanceFrom(Point<T> other) const {
		return Common::pythagoreanHypotenuse(X - other.X, Y - other.Y);
	}

	template<typename T>
	T Point<T>::length() const {
		return Common::pythagoreanHypotenuse(X, Y);
	}

	template <typename T>
	float Point<T>::getAngle(const Point<T>& origo, const Point<T>& other, Common::SteeringDir dir) {
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
			angle = -1 * getAngle(origo, Point<T>(2 * origo.X - other.X, other.Y), Common::SteeringDir::LEFT);
		}
		return angle;
	}

	template<typename T>
	Point<T> Point<T>::average(const Point<T>& p1, const Point<T>& p2) {
		return Point<T>((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2);
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

}
#endif	// POINT_HPP
