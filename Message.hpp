#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Stores code and value of a message.
	*/
	class Message {

	public:

		enum CODE {
			ACK_ = 0,			// for acknowledgements
			Speed = 1,          // [cm/sec] (>0 means FORWARD)
			SteeringAngle = 2,  // [rad] (>0 means LEFT)
			DriveMode = 3		// values in Common::DriveMode
		};

		static const Message ACK;

	private:
		CODE code;
		ByteArray<4> value;

	public:

		Message() {}

		Message(CODE _code, const ByteArray<4>& _value);

		Message(CODE _code, int _value);

		Message(CODE _code, float _value);

		Message& operator=(const Message& other) {
			setCode(other.code);
			setValue(other.value);
			return *this;
		}

		Message(const Message& other) {
			*this = other;
		}

		CODE getCode() const {
			return code;
		}

		void setCode(CODE _code) {
			code = _code;
		}

		byte getCodeAsByte() const {
			return static_cast<byte>(code);
		}

		ByteArray<4> getValue() const {
			return value;
		}

		void setValue(const ByteArray<4>& _value) {
			value = _value;
		}

		void setValue(const byte _value[]) {
			value = ByteArray<4>(_value);
		}

		int getValueAsInt() const {
			return value.asInteger();
		}

		void setValue(int _value) {
			ByteArray<4>::fromInteger(_value, value);
		}

		float getValueAsFloat() const {
			return value.asFloat();
		}

		void setValue(float _value) {
			ByteArray<4>::fromFloat(_value, value);
		}

		static byte codeToByte(CODE _code) {
			return static_cast<byte>(_code);
		}

		static CODE byteToCode(byte b) {
			return static_cast<CODE>(b);
		}

		ByteArray<COMM_MESSAGE_SIZE> toBytes() const;

		static void fromBytes(const ByteArray<COMM_MESSAGE_SIZE>& bytes, Message& dest);

#if __DEBUG
		String toString() const;
#endif // __DEBUG
	};
}


#endif	// MESSAGE_HPP

