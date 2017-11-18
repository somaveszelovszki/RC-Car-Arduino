#ifndef RC_CAR__POINT2__HPP
#define RC_CAR__POINT2__HPP

#include "ByteArray.hpp"

namespace rc_car {
	template <typename T>
	class Point2 {
	public:
		T X;
		T Y;

		static const Point2<T> ORIGO;
		Point2<T>() {}
		Point2<T>(T x, T y) : X(x), Y(y) {}

		Point2<T>& operator=(const Point2<T>& other);

		Point(const Point2<T>& other) {
			*this = other;
		}

		Point2<T> operator+(const Point2<T>& other) const;
		Point2<T> operator-(const Point2<T>& other) const;
		Point2<T>& operator+=(const Point2<T>& other);
		Point2<T>& operator-=(const Point2<T>& other);

		Point2<T> operator*(const T& c) const;
		Point2<T> operator/(const T& c) const;
		Point2<T>& operator*=(const T& c);
		Point2<T>& operator/=(const T& c);

		friend Point2<T> operator*(const T& c, const Point2<T>& p) {
			return p * c;
		}

		T distanceFrom(Point2<T> other) const;

		T length() const;

		static float getAngle(const Point2<T>& origo, const Point2<T>& other, Common::SteeringDir dir);

		/*
		Calculates steering angle of given point using this point as the origo.
		NOTE: Steering angle is always positive for LEFT, and always negative for RIGHT steering direction!
		*/
		float getAngle(const Point2<T>& other, Common::SteeringDir dir) const {
			return Point2<T>::getAngle(*this, other, dir);
		}

		static Point2<T> average(const Point2<T>& p1, const Point2<T>& p2);

		ByteArray<2> toByteArray() const;

		static Point2<T> fromByteArray(const ByteArray<2>& bytes);
	};



	template <typename T>
	const Point2<T> Point2<T>::ORIGO(static_cast<T>(0), static_cast<T>(0));

	template<typename T>
	Point2<T>& Point2<T>::operator=(const Point2<T>& other) {
		X = other.X;
		Y = other.Y;
	}

	template<typename T>
	Point2<T> Point2<T>::operator+(const Point2<T>& other) const {
		return Point2<T>(X + other.X, Y + other.Y);
	}

	template<typename T>
	Point2<T> Point2<T>::operator-(const Point2<T>& other) const {
		return Point2<T>(X - other.X, Y - other.Y);
	}

	template<typename T>
	Point2<T>& Point2<T>::operator+=(const Point2<T>& other) {
		X += other.X;
		Y += other.Y;
		return *this;
	}

	template<typename T>
	Point2<T>& Point2<T>::operator-=(const Point2<T>& other) {
		X -= other.X;
		Y -= other.Y;
		return *this;
	}

	template<typename T>
	Point2<T> Point2<T>::operator*(const T & c) const {
		return Point2<T>(X * c, Y * c);
	}

	template<typename T>
	Point2<T> Point2<T>::operator/(const T & c) const {
		return Point2<T>(X / c, Y / c);
	}

	template<typename T>
	Point2<T>& Point2<T>::operator*=(const T & c) {
		X *= c;
		Y *= c;
		return *this;
	}

	template<typename T>
	Point2<T>& Point2<T>::operator/=(const T & c) {
		X /= c;
		Y /= c;
		return *this;
	}

	template<typename T>
	T Point2<T>::distanceFrom(Point2<T> other) const {
		return Common::pythagoreanHypotenuse(X - other.X, Y - other.Y);
	}

	template<typename T>
	T Point2<T>::length() const {
		return Common::pythagoreanHypotenuse(X, Y);
	}

	template <typename T>
	float Point2<T>::getAngle(const Point2<T>& origo, const Point2<T>& other, Common::SteeringDir dir) {
		float angle;

		switch (dir) {
		case Common::SteeringDir::LEFT:
			if (Common::areEqual(origo.X, other.X))
				angle = other.Y > origo.Y ? static_cast<float>(M_PI_2) : 3 * static_cast<float>(M_PI_2);
			else if (other.X > origo.X)
				angle = other.Y >= origo.Y ?
				static_cast<float>(atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))))
				: static_cast<float>(2 * M_PI + atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))));
			else
				angle = static_cast<float>(M_PI + atan(static_cast<double>((other.Y - origo.Y) / (other.X - origo.X))));
			break;

		case Common::SteeringDir::RIGHT:
			angle = -1 * getAngle(origo, Point2<T>(2 * origo.X - other.X, other.Y), Common::SteeringDir::LEFT);
		}

		return angle;
	}

	template<typename T>
	Point2<T> Point2<T>::average(const Point2<T>& p1, const Point2<T>& p2) {
		return Point2<T>((p1.X + p2.X) / 2, (p1.Y + p2.Y) / 2);
	}

	template<typename T>
	ByteArray<2> Point2<T>::toByteArray() const {
		ByteArray<2> result;
		// maps X and Y coordinates to fit into a byte
		int _X = Common::incarcerate(static_cast<int>(X * 128 / ULTRA_MAX_DIST), -128, 127),
			_Y = Common::incarcerate(static_cast<int>(Y * 128 / ULTRA_MAX_DIST), -128, 127);

		result[0] = static_cast<byte>(_X);
		result[1] = static_cast<byte>(_Y);

		return result;
	}

	template<typename T>
	Point2<T> Point2<T>::fromByteArray(const ByteArray<2>& bytes) {
		return Point2<T>(
			static_cast<T>(static_cast<int>(bytes[0]) * ULTRA_MAX_DIST / 128.0f),
			static_cast<T>(static_cast<int>(bytes[1]) * ULTRA_MAX_DIST / 128.0f)
		);
	}

	typedef Point2<float>	Point2f, Vec2f;
	typedef Point2<int>		Point2i, Vec2i;
}
#endif // RC_CAR__POINT2__HPP
