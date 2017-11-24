#ifndef RC_CAR__MESSAGE__HPP
#define RC_CAR__MESSAGE__HPP

#include "ByteArray.hpp"

namespace rc_car {
	/** @brief Stores a Message object.
    Handles code values and different types as data.
	*/
	class Message {

	public:

        /** @brief Defines message priority.
        */
        enum Prio {
            LOW_PRIO = 0,
            HIGH_PRIO
        };

        /** @brief Data value for indicating boolean TRUE.
        */
		static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_VALUE_TRUE;

        /** @brief Data value for indicating boolean FALSE.
        */
		static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_VALUE_FALSE;

        /** @brief Message separator byte array.
        */
        static const ByteArray<COMM_MSG_SEPARATOR_LENGTH> SEPARATOR;

		enum CODE {
			ACK_				= 0b00000000,		// for acknowledgements
			Speed				= 0b00000001,       // [cm/sec] (>0 means FORWARD)
			SteeringAngle		= 0b00000010,		// [degree] (>0 means LEFT)
			DriveMode			= 0b00000011,		// values in Common::DriveMode

			Ultra0_1_EnvPoint	= 0b00001000,       // sensed points for sensors 0 and 1
			Ultra2_3_EnvPoint	= 0b00001001,       // sensed points for sensors 2 and 3
			Ultra4_5_EnvPoint	= 0b00001010,       // sensed points for sensors 4 and 5
			Ultra6_7_EnvPoint	= 0b00001011,       // sensed points for sensors 6 and 7
			Ultra8_9_EnvPoint	= 0b00001100,       // sensed points for sensors 8 and 9
			Ultra10_11_EnvPoint	= 0b00001101,       // sensed points for sensors 10 and 11
			Ultra12_13_EnvPoint	= 0b00001110,       // sensed points for sensors 12 and 13
			Ultra14_15_EnvPoint	= 0b00001111,       // sensed points for sensors 14 and 15
			EnableEnvironment	= 0b00010000        // enable/disable environment point sending
		};

        /** @brief Pre-defined acknowledge message.
        */
		static const Message ACK;

	private:
        /** @brief The message code.
        */
		CODE code;

        /** @brief The message data.
        */
		ByteArray<COMM_MSG_DATA_LENGTH> data;

        /** @brief The message priority.
        */
        Prio prio;

	public:

        /** @brief Constructor - set priority to LOW_PRIO.
        */
		Message() : prio(LOW_PRIO) {}

        /** @brief Constructor - sets code, byte array data and priority.

        @param _code The code to set.
        @param _data The byte array data to set.
        @param _prio The priority to set.
        */
        Message(CODE _code, const ByteArray<4>& _data, Prio _prio = LOW_PRIO) : code(_code), prio(_prio) {
            setData(_data);
        }

        /** @brief Constructor - sets code, integer data and priority.

        @param _code The code to set.
        @param _data The integer data to set.
        @param _prio The priority to set.
        */
		Message(CODE _code, int32_t _data, Prio _prio = LOW_PRIO) : code(_code), prio(_prio) {
            setData(_data);
        }

        /** @brief Constructor - sets code, float data and priority.

        @param _code The code to set.
        @param _data The float data to set.
        @param _prio The priority to set.
        */
		Message(CODE _code, float _data, Prio _prio = LOW_PRIO) : code(_code), prio(_prio) {
            setData(_data);
        }

        /** @brief Copies code, data and priority of the other message.

        @param other The other message.
        @returns This message.
        */
		Message& operator=(const Message& other) {
			setCode(other.code);
			setData(other.data);
            setPriority(other.prio);
			return *this;
		}

        /** @brief Copy constructor - copies code, data and priority.

        @param other The other message.
        */
		Message(const Message& other) {
			*this = other;
		}

        /** @brief Gets message code.

        @returns The message code.
        */
		CODE getCode() const {
			return code;
		}

        /** @brief Sets message code.

        @param _code The message code to set.
        */
		void setCode(CODE _code) {
			code = _code;
		}

        /** @brief Gets message code as a byte.

        @returns The message code as a byte.
        */
		byte getCodeAsByte() const {
			return static_cast<byte>(code);
		}

        /** @brief Sets message code.

        @param _data The byte code to set.
        */
        void setCode(byte _code) {
            code = static_cast<CODE>(_code);
        }

        /** @brief Gets message data.

        @returns The message data.
        */
		ByteArray<4> getData() const {
			return data;
		}

        /** @brief Sets message data.

        @param _data The message data to set.
        */
		void setData(const ByteArray<4>& _data) {
			data = _data;
		}

        /** @brief Sets message data.

        @param _data The message data to set.
        */
		void setData(const byte _data[]) {
			data = ByteArray<4>(_data);
		}

        /** @brief Gets message data as integer.

        @returns The message as integer.
        */
		int32_t getDataAsInt() const {
			return data.asInteger();
		}

        /** @brief Sets message data.

        @param _data The integer data to set.
        */
		void setData(int32_t _data) {
			ByteArray<4>::fromInteger(_data, data);
		}

        /** @brief Gets message data as float.

        @returns The message data as float.
        */
		float getDataAsFloat() const {
			return data.asFloat();
		}

        /** @brief Sets message data.

        @param _data The float data to set.
        */
		void setData(float _data) {
			ByteArray<4>::fromFloat(_data, data);
		}

        /** @brief Gets message data as boolean.

        @returns The message data as boolean.
        */
		bool getDataAsBool() const {
			return data == BOOL_VALUE_TRUE;
		}

        /** @brief Sets message data.

        @param _data The boolean data to set.
        */
		void setData(bool _data) {
			data = _data ? BOOL_VALUE_TRUE : BOOL_VALUE_FALSE;
		}

        /** @brief Gets message priority.

        @returns The message priority.
        */
        Prio getPriority() const {
            return prio;
        }

        /** @brief Sets message priority.

        @param _prio The message priority to set.
        */
        void setPriority(Prio _prio) {
            prio = _prio;
        }

        /** @brief Converts message to byte array.

        @returns The message as a ByteArray object.
        */
		ByteArray<COMM_MSG_LENGTH> toBytes() const;

        /** @brief Creates message from a byte array.

        @param bytes The source ByteArray object.
        @param result The result message.
        */
		static void fromBytes(const ByteArray<COMM_MSG_LENGTH>& bytes, Message& result);

#if __DEBUG
        /** @brief Builds String from Message object.

        @returns The message as a String.
        */
		String toString() const;
#endif // __DEBUG
	};
}

#endif // RC_CAR__MESSAGE__HPP
