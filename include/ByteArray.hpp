#ifndef RC_CAR__BYTE_ARRAY__HPP
#define RC_CAR__BYTE_ARRAY__HPP

#include "Common.hpp"

namespace rc_car {
/** @brief Stores byte array of given size. Implements byte array to int and float, and vica versa transformations.

@tparam size Size of the byte array.
*/
template <int size>
class ByteArray {
private:
    /** @brief Array of bytes containing the data.
    */
    byte value[size];

public:
    /** @brief Default constructor - does not initialize array values.
    */
    ByteArray() {}

    /** @brief Constructor - converts integer value to ByteArray.

    @param _value The integer value to convert to ByteArray.
    */
    explicit ByteArray(int32_t _value) {
        fromInteger(_value, *this);
    }

    /** @brief Constructor - converts float value to ByteArray.

    @param _value The float value to convert to ByteArray.
    */
    explicit ByteArray(float _value) {
        fromFloat(_value, *this);
    }

    /** @brief Constructor - converts array of byte values to ByteArray.

    @param _value The array of byte values to convert to ByteArray.
    */
    explicit ByteArray(const byte _value[]) {
        setValue(_value);
    }

    /** @brief Copies data to destination byte array.

    @param dest The destination byte array.
    */
    void getValue(byte dest[]) const {
        Common::copy<size>(value, dest);
    }

    /** @brief Copies data from source byte array.

    @param _value The source byte array.
    */
    void setValue(const byte _value[]) {
        Common::copy<size>(_value, value);
    }

    /** @brief Copies data from the other ByteArray.

    @param other The other ByteArray.
    @returns This ByteArray.
    */
    ByteArray<size>& operator=(const ByteArray<size>& other) {
        setValue(other.value);
        return *this;
    }

    /** @brief Casts ByteArray to const byte array.

    @returns The byte array data.
    */
    explicit operator const byte*() const {
        return value;
    }

    /** @brief Casts ByteArray to byte array.

    @returns The byte array data.
    */
    explicit operator byte*() {
        return value;
    }

    /** @brief Gets data at given position.

    @returns The byte at the given position.
    */
    byte& operator[](int pos) {
        return value[pos];
    }

    /** @brief Gets data at given position.

    @returns The byte at the given position.
    */
    const byte& operator[](int pos) const {
        return value[pos];
    }

    /** @brief Checks if two ByteArray objects have equal values.

    @param other The other ByteArray object.
    @returns Boolean value indicating if the two ByteArray objects have equal values.
    */
    bool operator==(const ByteArray<size>& other) const {
        bool eq = true;
        for (int i = 0; eq && i < size; ++i)
            eq = (*this)[i] == other[i];
        return eq;
    }

    /** @brief Checks if two ByteArray objects do not have equal values.

    @param other The other ByteArray object.
    @returns Boolean value indicating if the two ByteArray objects do not have equal values.
    */
    bool operator!=(const ByteArray<size>& other) const {
        return !(*this == other);
    }

    /** @brief Converts integer value to ByteArray.

    @param _value The integer value to convert to ByteArray.
    @param other The destination ByteArray object.
    */
    static void fromInteger(int32_t _value, ByteArray<size>& dest) {
        Common::intToBytes(_value, dest.value);
    }

    /** @brief Converts float value to ByteArray.

    @param _value The float value to convert to ByteArray.
    @param other The destination ByteArray object.
    */
    static void fromFloat(float _value, ByteArray<size>& dest) {
        Common::floatToBytes(_value, dest.value);
    }

    /** @brief Gets data as integer value.

    @returns Data as integer value.
    */
    int32_t asInteger() const {
        return Common::bytesToInt(value);
    }

    /** @brief Gets data as float value.

    @returns Data as float value.
    */
    float asFloat() const {
        return Common::bytesToFloat(value);
    }

    /** @brief Concats two ByteArray objects.

    @tparam size2 Size of the other ByteArray object.
    @param other The other ByteArray object.
    @param dest The destination ByteArray object.
    */
    template <int size2>
    void concat(const ByteArray<size2>& other, ByteArray<size + size2>& dest) const {
        Common::arrayConcat<size, size2>(static_cast<const byte*>(*this), static_cast<const byte*>(other), static_cast<byte*>(dest));
    }

    /** @brief Gets subarray of given length starting at given position.

    @tparam size2 Size of the subarray.
    @param startIndex Start index of subarray.
    @returns The subarray.
    */
    template <int size2>
    ByteArray<size2> subArray(int startIndex) {
        return ByteArray<size2>(&value[startIndex]);
    }

    /** @brief Appends byte at the end of this byte array, and stores result in the destination ByteArray object.

    @tparam b The byte to append.
    @param dest The destination ByteArray object.
    */
    void append(byte b, ByteArray<size + 1>& dest) const {
        Common::copy<size>(value, static_cast<byte*>(dest));
        dest[size] = b;
    }

    /** @brief Concats two ByteArray objects.

    @tparam size2 Size of the other ByteArray object.
    @param other The other ByteArray object.
    @returns The result ByteArray object.
    */
    template <int size2>
    ByteArray<size + size2> operator+(const ByteArray<size2>& other) const {
        ByteArray<size + size2> result;
        concat(other, result);
        return result;
    }

    /** @brief Appends byte at the end of this byte array, and returns result ByteArray object.

    @tparam b The byte to append.
    @returns The result ByteArray object.
    */
    ByteArray<size + 1> operator+(byte b) const {
        ByteArray<size + 1> result;
        append(b, result);
        return result;
    }

    /** @brief Appends byte at the beginning of the byte array, and returns result ByteArray object.

    @tparam b The byte to append.
    @param bytes The source ByteArray object.
    @returns The result ByteArray object.
    */
    friend ByteArray<1 + size> operator+(byte b, const ByteArray<size>& bytes) {
        return ByteArray<1>(&b) + bytes;
    }

    /** @brief Shifts bytes to the left (shifts towards MSB).
    NOTE: Values are not rotated, rightmost elements will be filled with ZEROs!

    @tparam byteShift Number of bytes to shift to the left.
    */
    void shiftBytesLeft(int byteShift) {
        for (int i = size - 1; i >= byteShift; --i)
            (*this)[i] = (*this)[i - byteShift];

        for (int i = byteShift - 1; i >= 0; --i)
            (*this)[i] = static_cast<byte>(0);
    }

    /** @brief Shifts bytes to the right (shifts towards LSB).
    NOTE: Values are not rotated, leftmost elements will be filled with ZEROs!

    @tparam byteShift Number of bytes to shift to the right.
    */
    void shiftBytesRight(int byteShift) {
        for (int i = 0; i < size - byteShift; ++i)
            (*this)[i] = (*this)[i + byteShift];

        for (int i = byteShift; i < size; ++i)
            (*this)[i] = static_cast<byte>(0);
    }

    /** @brief Finds index of first occurence of integer value.
    NOTE: Length of the byte array must be at least 4!

    @param _value The integer value to look for in the byte array.
    */
    int indexOf(int32_t _value) const {
        int index = -1;
        for (int i = 0; index == -1 && i <= size - 4; ++i)
            if (Common::bytesToInt(value, i) == _value)
                index = i;
        return index;
    }

    /** @brief Finds index of first occurence of float value.
    NOTE: Length of the byte array must be at least 4!

    @param _value The float value to look for in the byte array.
    */
    int indexOf(float _value) const {
        int index = -1;
        for (int i = 0; index > -1 && i < size - 4; ++i)
            if (Common::bytesToFloat(value, i) == _value)
                index = i;
        return index;
    }

#if __DEBUG__
    /** @brief Prints ByteArray to the Serial port.
    */
    void print() const {
        DEBUG_print("[ ");
        for (int i = 0; i < size - 1; ++i) {
            DEBUG_print(value[i]);
            DEBUG_print(", ");
        }
        if (size > 0)
            DEBUG_print(value[size - 1]);

        DEBUG_print(" ]");
    }
#endif // __DEBUG__
};
}

#endif // RC_CAR__BYTE_ARRAY__HPP
