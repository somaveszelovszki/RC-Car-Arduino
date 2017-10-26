#ifndef BYTE_ARRAY_HPP
#define BYTE_ARRAY_HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Stores byte array of given size. Implements byte array to int and float, and vica versa transformations.

	@tparam size Size of the byte array.
	*/
	template <int size>
	class ByteArray {
	private:
		byte value[size];

	public:
		ByteArray() {}

		explicit ByteArray(int32_t _value) {
			fromInteger(_value, *this);
		}

		explicit ByteArray(float _value) {
			fromFloat(_value, *this);
		}

		explicit ByteArray(const byte _value[size]) {
			setValue(_value);
		}

		void getValue(byte dest[size]) const {
			Common::arrayCopy<size>(value, dest);
		}

		void setValue(const byte _value[size]) {
			Common::arrayCopy<size>(_value, value);
		}

		ByteArray<size>& operator=(const ByteArray<size>& other) {
			setValue(other.value);
			return *this;
		}

		explicit operator const byte*() const {
			return value;
		}

		explicit operator byte*() {
			return value;
		}

		byte& operator[](int pos) {
			return value[pos];
		}

		const byte& operator[](int pos) const {
			return value[pos];
		}

		static void fromInteger(int32_t _value, ByteArray<size>& dest) {
			Common::intToBytes(_value, dest.value);
		}

		static void fromFloat(float _value, ByteArray<size>& dest) {
			Common::floatToBytes(_value, dest.value);
		}

		int32_t asInteger() const {
			return Common::bytesToInt(value);
		}

		float asFloat() const {
			return Common::bytesToFloat(value);
		}

		template <int size2>
		ByteArray<size + size2> concat(const ByteArray<size2>& other, ByteArray<size + size2>& dest) const {
			Common::arrayConcat<size, size2>(static_cast<const byte*>(*this), static_cast<const byte*>(other), static_cast<byte*>(dest));
		}

		void append(byte b, ByteArray<size + 1>& dest) const {
			Common::arrayCopy<size>(value, static_cast<byte*>(dest));
			dest[size] = b;
		}

		template <int size2>
		ByteArray<size + size2> operator+(const ByteArray<size2>& other) const {
			ByteArray<size + size2> result;
			this->concat(other, result);
			return result;
		}

		ByteArray<size + 1> operator+(byte b) const {
			ByteArray<size + 1> result;
			this->append(b, result);
			return result;
		}

		friend ByteArray<1 + size> operator+(byte b, const ByteArray<size>& bytes) {
			return ByteArray<1>(&b) + bytes;
		}

		void shiftBytesLeft(int byteShift) {
			for (int i = size - 1; i >= byteShift; --i)
				(*this)[i] = (*this)[i - byteShift];

			for (int i = byteShift - 1; i >= 0; --i)
				(*this)[i] = static_cast<byte>(0);
		}

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

#if __DEBUG
		String toString() const;
#endif // __DEBUG
	};

#if __DEBUG
	template<int size>
	String ByteArray<size>::toString() const {
		String str("[ ");
		for (int i = 0; i < size - 1; ++i) {
			str += value[i];
			str += ", ";
		}
		if (size > 0)
			str += value[size - 1];

		str += " ]";
		return str;
	}
#endif // __DEBUG
}

#endif // BYTE_ARRAY_HPP