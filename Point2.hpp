#ifndef RC_CAR__POINT2__HPP
#define RC_CAR__POINT2__HPP

#include "ByteArray.hpp"

namespace rc_car {
/** @brief Template implementation for 2-dimensional points.

@tparam T Numeric type of the coordinates.
*/
template <typename T>
class Point2;

template <typename T>
using Vec2 = Point2<T>;

template <typename T>
class Point2 {
public:
    /** @brief The X coordinate.
    */
    T X;

    /** @brief The Y coordinate.
    */
    T Y;

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

    /** @brief Checks if two points are equal

    @param other The other point.
    @returns Boolean value indicating if the two points are equal.
    */
    bool operator==(const Point2<T>& other) const {
        return X == other.X && Y == other.Y;
    }

    /** @brief Checks if two points are not equal

    @param other The other point.
    @returns Boolean value indicating if the two points are not equal.
    */
    bool operator!=(const Point2<T>& other) const {
        return X != other.X || Y != other.Y;
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

    /** @brief Casts point to another type.

    @returns Point cast to another type.
    */
    template <typename T2>
    operator Point2<T2>() const {
        return Point2<T2>(static_cast<T>(X), static_cast<T>(Y));
    }

    /** @brief Calculates distance between the two points.

    @param other The other point.
    @returns The distance between the two points.
    */
    T distance(Point2<T> other) const {
        return Common::pythag(X - other.X, Y - other.Y);
    }

    /** @brief Calculates length of the point vector.

    @returns The length of the point vector.
    */
    T length() const {
        return Common::pythag(X, Y);
    }

    /** @brief Calculates steering angle of given vector using this point as the origo.

    @param other The vector.
    @param dir Indicates which is the positive steering direction.
    */
    float getAngle(const Vec2<T>& other, Common::RotationDir dir) const;

    /** @brief Calculates weighted average of the two points.

    @param other The other point.
    @param otherWeight The weight of the other point relative to this point - 1 by default.
    @returns The average of the two points.
    */
    Point2<T> average(const Point2<T>& other, float otherWeight = 1.0f) const {
        float weightSum = 1.0f + otherWeight;
        return Point2<T>((X + other.X * otherWeight) / weightSum, (Y + other.Y + otherWeight) / weightSum);
    }

    /** @brief Converts point to byte array.

    @param The result ByteArray object.
    */
    void toByteArray(ByteArray<2>& result) const;

    /** @brief Creates point from byte array.

    @param bytes The source ByteArray object.
    @returns The point created from the byte array.
    */
    static Point2<T> fromByteArray(const ByteArray<2>& bytes);

    // NOTE: numPoints must be at least 1
    static void bbox(const Point2<T> points[], int numPoints, Point2<T> *pBottomLeft, Point2<T> *pTopRight);

    Common::Sign getAngleSign(const Vec2<T>& other, Common::RotationDir dir) const;

    bool isInside(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) const;

#if __DEBUG__
    void print() const {
        DEBUG_print("(");
        DEBUG_print(X);
        DEBUG_print(", ");
        DEBUG_print(Y);
        DEBUG_print(")");
    }
#endif // __DEBUG__

};

template <typename T>
float Point2<T>::getAngle(const Vec2<T>& other, Common::RotationDir dir) const {
    float angle;

    switch (dir) {
    case Common::RotationDir::LEFT:
        if (Common::eq(X, other.X))
            angle = other.Y > Y ? M_PI_2 : 3 * M_PI_2;
        else if (other.X > X)
            angle = other.Y >= Y ? atanf((other.Y - Y) / (other.X - X)) : 2 * M_PI + atanf((other.Y - Y) / (other.X - X));
        else
            angle = M_PI + atanf((other.Y - Y) / (other.X - X));
        break;

    case Common::RotationDir::RIGHT:
        angle = -1 * getAngle(Point2<T>(2 * X - other.X, other.Y), Common::RotationDir::LEFT);
    }

    return angle;
}

template<typename T>
void Point2<T>::toByteArray(ByteArray<2>& result) const {
    // maps X and Y coordinates to fit into a byte
    int _X = Common::incarcerate(static_cast<int>(X * 128 / ULTRA_MAX_DIST), -128, 127),
        _Y = Common::incarcerate(static_cast<int>(Y * 128 / ULTRA_MAX_DIST), -128, 127);

    result[0] = static_cast<byte>(_X);
    result[1] = static_cast<byte>(_Y);
}

template<typename T>
Point2<T> Point2<T>::fromByteArray(const ByteArray<2>& bytes) {
    return Point2<T>(
        static_cast<T>(static_cast<int>(bytes[0]) * ULTRA_MAX_DIST / 128.0f),
        static_cast<T>(static_cast<int>(bytes[1]) * ULTRA_MAX_DIST / 128.0f)
        );
}

template<typename T>
void Point2<T>::bbox(const Point2<T> points[], int numPoints, Point2<T> *pBottomLeft, Point2<T> *pTopRight) {
    *pBottomLeft = *pTopRight = points[0];
    for (int i = 1; i < numPoints; ++i) {
        Point2<T> p = points[i];

        if (p.X < pBottomLeft->X)
            pBottomLeft->X = p.X;
        else if (p.X > pTopRight->X)
            pTopRight->X = p.X;

        if (p.Y < pBottomLeft->Y)
            pBottomLeft->Y = p.Y;
        else if (p.Y > pTopRight->Y)
            pTopRight->Y = p.Y;
    }
}

template<typename T>
Common::Sign Point2<T>::getAngleSign(const Vec2<T>& other, Common::RotationDir dir) const {
    float m;
    if (Common::isZero(X)) {
        m = Y > 0 ? -other.X : other.X;
    } else if (Common::isZero(other.X)) {
        m = other.Y > 0 ? X : -X;
    } else {
        m = Y / static_cast<float>(X) - other.Y / static_cast<float>(other.X);
    }

    Common::Sign sign = Common::sgn(m);
    if (dir == Common::RotationDir::RIGHT)
        sign = static_cast<Common::Sign>(static_cast<int>(sign) * -1);
    return sign;
}

template<typename T>
bool Point2<T>::isInside(const Point2<T>& a, const Point2<T>& b, const Point2<T>& c) const {
    Vec2<T> ap = *this - a, bp = *this - b, cp = *this - c,
        ab = b - a, bc = c - b, ca = a - c;

    Common::RotationDir dir = Common::RotationDir::LEFT;

    Common::Sign sA = ab.getAngleSign(ap, dir), sB = bc.getAngleSign(bp, dir), sC = ca.getAngleSign(cp, dir);

    // if point is in line of the side, determines it if it is outside or on the line
    if (sA == Common::Sign::ZERO && !Common::isBtw(ap.X, static_cast<T>(0), ab.X))
        return false;

    if (sB == Common::Sign::ZERO && !Common::isBtw(bp.X, static_cast<T>(0), bc.X))
        return false;

    if (sC == Common::Sign::ZERO && !Common::isBtw(cp.X, static_cast<T>(0), ca.X))
        return false;

    // sum can be +-3 when all values are either POSITIVE or NEGATIVE,
    // or it can be +-2 if one of the results is ZERO
    return abs(static_cast<int8_t>(sA) + static_cast<int8_t>(sB) + static_cast<int8_t>(sC)) >= static_cast<int8_t>(2);
}

typedef Point2<float>       Point2f, Vec2f;
typedef Point2<uint8_t>     Point2ui8, Vec2ui8;

}
#endif // RC_CAR__POINT2__HPP
