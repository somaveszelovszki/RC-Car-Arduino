#ifndef RC_CAR__POINT2__HPP
#define RC_CAR__POINT2__HPP

#include "ByteArray.hpp"

namespace rc_car {
    /** @brief Template implementation for 2-dimensional points.

    @tparam T Numeric type of the coordinates.
    */
	template <typename T>
	class Point2 {
	public:
        /** @brief The X coordinate.
        */
		T X;

        /** @brief The Y coordinate.
        */
		T Y;

        /** @brief Contains the ORIGO.
        */
		static const Point2<T> ORIGO;

        /** @brief Constructor - does not initializes coordinates.
        */
		Point2<T>() {}

        /** @brief Constructor - initializes X and Y coordinates.

        @param _X The X coordinate.
        @param _Y The Y coordinate.
        */
		Point2<T>(T _X, T _Y) : X(_X), Y(_Y) {}

        /** @brief Copies coordinates of the other point.

        @param other The other point.
        @returns This point.
        */
        const Point2<T>& operator=(const Point2<T>& other) {
            X = other.X;
            Y = other.Y;

            return *this;
        }

        /** @brief Copy constructor - copies coordinates of the other point.

        @param other The other point.
        */
		Point2(const Point2<T>& other) {
			*this = other;
		}

        /** @brief Adds coordinates of this and the other point.

        @param other The other point.
        @returns The result of the addition.
        */
        Point2<T> operator+(const Point2<T>& other) const {
            return Point2<T>(X + other.X, Y + other.Y);
        }

        /** @brief Subtracts coordinates of the other point from the coordinates of this point.

        @param other The other point.
        @returns The result of the subtraction.
        */
        Point2<T> operator-(const Point2<T>& other) const {
            return Point2<T>(X - other.X, Y - other.Y);
        }

        /** @brief Adds coordinates of this and the other point and stores the result in this point.

        @param other The other point.
        @returns This point.
        */
        Point2<T>& operator+=(const Point2<T>& other) {
            X += other.X;
            Y += other.Y;
            return *this;
        }

        /** @brief Subtracts coordinates of the other point from the coordinates of this point and stores the result in this point.

        @param other The other point.
        @returns This point.
        */
        Point2<T>& operator-=(const Point2<T>& other) {
            X -= other.X;
            Y -= other.Y;
            return *this;
        }

        /** @brief Multiplies coordinates of the point with the given constant.

        @param c The constant.
        @returns The result of the multiplication.
        */
        Point2<T> operator*(const T& c) const {
            return Point2<T>(X * c, Y * c);
        }

        /** @brief Divides coordinates of the point by the given constant.

        @param c The constant.
        @returns The result of the division.
        */
        Point2<T> operator/(const T& c) const {
            return Point2<T>(X / c, Y / c);
        }

        /** @brief Multiplies coordinates of the point with the given constant and stores the result in the point.

        @param c The constant.
        @returns This point.
        */
        Point2<T>& operator*=(const T& c) {
            X *= c;
            Y *= c;
            return *this;
        }

        /** @brief Divides coordinates of the point by the given constant and stores the result in the point.

        @param c The constant.
        @returns This point.
        */
        Point2<T>& operator/=(const T& c) {
            X /= c;
            Y /= c;
            return *this;
        }

        /** @brief Multiplies coordinates of the point with the given constant.

        @param c The constant.
        @param p The point.
        @returns The result of the multiplication.
        */
        friend Point2<T> operator*(const T& c, const Point2<T>& p) {
            return p * c;
        }

        /** @brief Calculates distance between the two points.

        @param other The other point.
        @returns The distance between the two points.
        */
        T distanceFrom(Point2<T> other) const {
            return Common::pythagoreanHypotenuse(X - other.X, Y - other.Y);
        }

        /** @brief Calculates length of the point vector.

        @returns The length of the point vector.
        */
        T length() const {
            return Common::pythagoreanHypotenuse(X, Y);
        }

		/** @brief Calculates steering angle of given point using this point as the origo.

        @param other The other point.
        @param dir Indicates which is the positive steering direction.
		*/
        float getAngle(const Point2<T>& other, Common::SteeringDir dir) const;

        /** @brief Calculates weighted average of the two points.

        @param other The other point.
        @param otherWeight The weight of the other point relative to this point - 1 by default.
        @returns The average of the two points.
        */
        Point2<T> averageWith(const Point2<T>& other, float otherWeight = 1.0f) const {
            float weightSum = 1.0f + otherWeight;
            return Point2<T>((X + other.X * otherWeight) / weightSum, (Y + other.Y + otherWeight) / weightSum);
        }

        /** @brief Converts point to byte array.

        @returns The point as a ByteArray object.
        */
		ByteArray<2> toByteArray() const;

        /** @brief Creates point from byte array.

        @param bytes The source ByteArray object.
        @returns The point created from the byte array.
        */
		static Point2<T> fromByteArray(const ByteArray<2>& bytes);
	};



	template <typename T>
	const Point2<T> Point2<T>::ORIGO(static_cast<T>(0), static_cast<T>(0));

	template <typename T>
	float Point2<T>::getAngle(const Point2<T>& other, Common::SteeringDir dir) const {
		float angle;

		switch (dir) {
		case Common::SteeringDir::LEFT:
			if (Common::areEqual(X, other.X))
				angle = other.Y > Y ? static_cast<float>(M_PI_2) : 3 * static_cast<float>(M_PI_2);
			else if (other.X > X)
				angle = other.Y >= Y ?
				static_cast<float>(atan(static_cast<double>((other.Y - Y) / (other.X - X))))
				: static_cast<float>(2 * M_PI + atan(static_cast<double>((other.Y - Y) / (other.X - X))));
			else
				angle = static_cast<float>(M_PI + atan(static_cast<double>((other.Y - Y) / (other.X - X))));
			break;

		case Common::SteeringDir::RIGHT:
			angle = -1 * getAngle(Point2<T>(2 * X - other.X, other.Y), Common::SteeringDir::LEFT);
		}

		return angle;
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
