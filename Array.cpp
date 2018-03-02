#include "Array.hpp"

using namespace rc_car;

#if ARRAY_1_EN
const byte Selector<1>::mask[8] = {
    static_cast<byte>(0b00000001),
    static_cast<byte>(0b00000010),
    static_cast<byte>(0b00000100),
    static_cast<byte>(0b00001000),
    static_cast<byte>(0b00010000),
    static_cast<byte>(0b00100000),
    static_cast<byte>(0b01000000),
    static_cast<byte>(0b10000000)
};
#endif // ARRAY_1_EN

#if ARRAY_2_EN
const byte Selector<2>::mask[4] = {
    static_cast<byte>(0b00000011),
    static_cast<byte>(0b00001100),
    static_cast<byte>(0b00110000),
    static_cast<byte>(0b11000000)
};
#endif // ARRAY_2_EN

#if ARRAY_4_EN
const byte Selector<4>::mask[2] = {
    static_cast<byte>(0b00001111),
    static_cast<byte>(0b11110000)
};
#endif // ARRAY_4_EN