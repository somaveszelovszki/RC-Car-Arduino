#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Stores code and data of a message.
	*/
	class Message {

	public:

		enum CODE {
			ACK_ = 0,			// for acknowledgements
			Speed = 1,          // [cm/sec] (>0 means FORWARD)
			SteeringAngle = 2,  // [rad] (>0 means LEFT)
			DriveMode = 3		// datas in Common::DriveMode
		};

		static const ByteArray<COMM_MSG_SEPARATOR_LENGTH> SEPARATOR;

		static const Message ACK;

	private:
		CODE code;
		ByteArray<COMM_MSG_DATA_LENGTH> data;

	public:

		Message() {}

		Message(CODE _code, const ByteArray<4>& _data);

		Message(CODE _code, int32_t _data);

		Message(CODE _code, float _data);

		Message& operator=(const Message& other) {
			setCode(other.code);
			setData(other.data);
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

		ByteArray<4> getData() const {
			return data;
		}

		void setData(const ByteArray<4>& _data) {
			data = _data;
		}

		void setData(const byte _data[]) {
			data = ByteArray<4>(_data);
		}

		int32_t getDataAsInt() const {
			return data.asInteger();
		}

		void setData(int32_t _data) {
			ByteArray<4>::fromInteger(_data, data);
		}

		float getDataAsFloat() const {
			return data.asFloat();
		}

		void setData(float _data) {
			ByteArray<4>::fromFloat(_data, data);
		}

		static byte codeToByte(CODE _code) {
			return static_cast<byte>(_code);
		}

		static CODE byteToCode(byte b) {
			return static_cast<CODE>(b);
		}

		ByteArray<COMM_MSG_LENGTH> toBytes() const;

		static void fromBytes(const ByteArray<COMM_MSG_LENGTH>& bytes, Message& dest);

#if __DEBUG
		String toString() const;
#endif // __DEBUG
	};
}

#endif	// MESSAGE_HPP