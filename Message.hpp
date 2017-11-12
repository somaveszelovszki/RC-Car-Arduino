#ifndef RC_CAR__MESSAGE__HPP
#define RC_CAR__MESSAGE__HPP

#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Stores code and data of a message.
	*/
	class Message {

	public:

		static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_VALUE_TRUE;
		static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_VALUE_FALSE;

		enum CODE {
			ACK_				= 0b00000000,		// for acknowledgements
			Speed				= 0b00000001,       // [cm/sec] (>0 means FORWARD)
			SteeringAngle		= 0b00000010,		// [degree] (>0 means LEFT)
			DriveMode			= 0b00000011,		// datas in Common::DriveMode

			Ultra0_1_EnvPoint	= 0b00001000,
			Ultra2_3_EnvPoint	= 0b00001001,
			Ultra4_5_EnvPoint	= 0b00001010,
			Ultra6_7_EnvPoint	= 0b00001011,
			Ultra8_9_EnvPoint	= 0b00001100,
			Ultra10_11_EnvPoint	= 0b00001101,
			Ultra12_13_EnvPoint	= 0b00001110,
			Ultra14_15_EnvPoint	= 0b00001111,
			EnableEnvironment	= 0b00010000
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

		bool getDataAsBool() const {
			return data == BOOL_VALUE_TRUE;
		}

		void setData(bool _data) {
			data = _data ? BOOL_VALUE_TRUE : BOOL_VALUE_FALSE;
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

#endif // RC_CAR__MESSAGE__HPP
