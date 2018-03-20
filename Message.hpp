#ifndef RC_CAR__MESSAGE__HPP
#define RC_CAR__MESSAGE__HPP

#include "ByteArray.hpp"

namespace rc_car {
/** @brief Stores a Message object.
Handles code values and different types as data.
*/
class Message {

public:

    /** @brief Data value for indicating boolean TRUE.
    */
    static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_TRUE;

    /** @brief Data value for indicating boolean FALSE.
    */
    static const ByteArray<COMM_MSG_DATA_LENGTH> BOOL_FALSE;

    /** @brief Message separator byte array.
    */
    static const ByteArray<COMM_MSG_SEPARATOR_LENGTH> SEPARATOR;

    enum CODE {
        ACK_ = 0,       // for acknowledgements
        Speed,          // [cm/sec] (>0 means FORWARD)
        SteeringAngle,  // [degree] (>0 means LEFT)
        DriveMode,      // values in Common::DriveMode
        CarPos,         // Absolute position of the car.
        CarAngle,       // Forward angle of the car (X axis is ZERO).
        RelEnvEn,       // enable/disable relative environment point sending
        RelEnvPoint,    // group for sensed relative points
        EnvGridEn,      // enable/disable environment grid sending
        EnvGrid,        // group for environment grid points (container's Y coordinate needs to be added to this number)
        NUM_CODES
    };

    class Code {
    public:
        CODE codeIdx;
        byte codeByte;
        byte matchPattern;

        Code() {}

        Code(CODE _codeIdx, byte _codeByte, byte _matchPattern = static_cast<byte>(0b11111111))
            : codeIdx(_codeIdx), codeByte(_codeByte), matchPattern(_matchPattern) {}

        static CODE apply(byte code);
    };

    static const Code CODES[CODE::NUM_CODES];

    /** @brief Pre-defined acknowledge message.
    */
    static const Message ACK;

private:
    /** @brief The message code.
    */
    Code code;

    /** @brief The message code byte - may be different from the code in case of code groups.
    */
    byte codeByte;

    /** @brief The message data.
    */
    ByteArray<COMM_MSG_DATA_LENGTH> data;

public:

    /** @brief Constructor - does not set elements.
    */
    Message() {}

    /** @brief Constructor - sets code and byte array data.

    @param _code The code to set.
    @param _data The byte array data to set.
    */
    Message(CODE _code, const ByteArray<4>& _data) {
        code = CODES[_code];
        codeByte = code.codeByte;
        setData(_data);
    }

    /** @brief Constructor - sets code and integer data.

    @param _code The code to set.
    @param _data The integer data to set.
    */
    Message(CODE _code, int32_t _data) {
        code = CODES[_code];
        codeByte = code.codeByte;
        setData(_data);
    }

    /** @brief Constructor - sets code and float data.

    @param _code The code to set.
    @param _data The float data to set.
    */
    Message(CODE _code, float _data) {
        code = CODES[_code];
        codeByte = code.codeByte;
        setData(_data);
    }

    /** @brief Constructor - sets code and byte array data.

    @param _codeByte The code byte to set.
    @param _data The byte array data to set.
    */
    Message(byte _codeByte, const ByteArray<4>& _data) {
        codeByte = _codeByte;
        code = CODES[Code::apply(_codeByte)];
        setData(_data);
    }

    /** @brief Constructor - sets code and byte array data.

    @param _codeByte The code byte to set.
    @param _data The byte array data to set.
    */
    Message(byte _codeByte, int32_t _data) {
        codeByte = _codeByte;
        code = CODES[Code::apply(_codeByte)];
        setData(_data);
    }

    /** @brief Constructor - sets code and byte array data.

    @param _codeByte The code byte to set.
    @param _data The byte array data to set.
    */
    Message(byte _codeByte, float _data) {
        codeByte = _codeByte;
        code = CODES[Code::apply(_codeByte)];
        setData(_data);
    }

    /** @brief Copies code, data of the other message.

    @param other The other message.
    @returns This message.
    */
    Message& operator=(const Message& other) {
        code = other.code;
        codeByte = other.codeByte;
        setData(other.data);
        return *this;
    }

    /** @brief Copy constructor - copies code, data of the other message.

    @param other The other message.
    */
    Message(const Message& other) {
        *this = other;
    }

    /** @brief Gets message code.

    @returns The message code.
    */
    CODE getCode() const {
        return code.codeIdx;
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

    /** @brief Sets code and code byte.

    @param _codeIdx The code to set.
    */
    void setCode(CODE _codeIdx) {
        code = CODES[_codeIdx];
        codeByte = code.codeByte;
    }

    /** @brief Sets code and code byte.

    @param _codeByte The code byte to set.
    */
    void setCode(byte _codeByte) {
        codeByte = _codeByte;
        code = CODES[Code::apply(codeByte)];
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
        return data == BOOL_TRUE;
    }

    /** @brief Sets message data.

    @param _data The boolean data to set.
    */
    void setData(bool _data) {
        data = _data ? BOOL_TRUE : BOOL_FALSE;
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

#if __DEBUG__
    /** @brief Prints Message object to the Serial port.
    */
    void print() const;
#endif // __DEBUG__
};
}

#endif // RC_CAR__MESSAGE__HPP
