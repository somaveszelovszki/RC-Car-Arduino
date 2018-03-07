#include "Common.hpp"

using namespace rc_car;

int32_t Common::bytesToInt(const byte bytes[], int startIndex) {
    return (static_cast<int32_t>(bytes[startIndex]) << 24)
        | (static_cast<int32_t>(bytes[startIndex + 1]) << 16)
        | (static_cast<int32_t>(bytes[startIndex + 2]) << 8)
        | static_cast<int32_t>(bytes[startIndex + 3]);
}

float Common::bytesToFloat(const byte bytes[], int startIndex) {
    int32_t intVal = bytesToInt(bytes, startIndex);
    return *reinterpret_cast<float*>(&intVal);
}

void Common::intToBytes(int32_t value, byte dest[]) {
    dest[0] = static_cast<byte>(value >> 24);
    dest[1] = static_cast<byte>(value >> 16);
    dest[2] = static_cast<byte>(value >> 8);
    dest[3] = static_cast<byte>(value);
}

void Common::floatToBytes(float value, byte dest[]) {
    int32_t intVal = *reinterpret_cast<int32_t*>(&value);
    intToBytes(intVal, dest);
}