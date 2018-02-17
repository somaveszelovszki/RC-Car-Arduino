#ifndef RC_CAR__ARRAY__HPP
#define RC_CAR__ARRAY__HPP

#include "Common.hpp"

namespace rc_car {

typedef uint8_t uint2_t;
typedef uint8_t uint4_t;

#if ARRAY_1_EN
/** @brief Mask for getting and setting bool element at given position in a byte container.
*/
static const byte mask1[8] = {
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
/** @brief Mask for getting and setting uint2_t element at given position in a byte container.
*/
static const byte mask2[4] = {
    static_cast<byte>(0b00000011),
    static_cast<byte>(0b00001100),
    static_cast<byte>(0b00110000),
    static_cast<byte>(0b11000000)
};
#endif // ARRAY_2_EN

#if ARRAY_4_EN
/** @brief Mask for getting and setting uint4_t element at given position in a byte container.
*/
static const byte mask4[2] = {
    static_cast<byte>(0b00001111),
    static_cast<byte>(0b11110000)
};
#endif // ARRAY_4_EN

/** @brief Selects type and mask array for given bit length.
*/
template <int B>
struct Selector;

/** @brief Selects type and mask array for bit length of 1.
*/
template <>
struct Selector<1> {
    typedef bool type;
    bool min = false;
    bool max = true;
};

/** @brief Selects type and mask array for bit length of 2.
*/
template <>
struct Selector<2> {
    typedef uint2_t type;
    uint2_t min = static_cast<uint2_t>(0);
    uint2_t max = static_cast<uint2_t>(3);
};

/** @brief Selects type and mask array for bit length of 4.
*/
template <>
struct Selector<4> {
    typedef uint4_t type;
    uint4_t min = static_cast<uint4_t>(0);
    uint4_t max = static_cast<uint4_t>(15);
};

/** @brief Array for storing values requiring less than __array_container_bits bits.
Values are stored in a byte array to reduce memory need.

@tparam B Bit length of the stored values (1, 2 or 4).
@tparam N Number of binary values stored in the array.
NOTE: (B * N) must be a multiple of 8!
*/
template <int B, int N>
class Array {
private:
    /** @brief Array container type - type of the chunks in which the data is stored.
    */
    typedef byte container_type;

    /** @brief Stored values per byte.
    */
    static int VPB = (sizeof(container_type) * 8) / B;

    /** @brief Byte array containing the binary values.
    */
    container_type data[N / VPB];

public:
    typedef Selector<B>::type type;
    static type dataMin = Selector<B>::min;
    static type dataMax = Selector<B>::max;

    /** @brief Gets value at given position.

    @param pos The position of the value to get.
    @returns The value of the element.
    */
    type get(int pos) const {
        return data[pos / VPB] & mask[pos % VPB];
    }

    /** @brief Sets element at given position to the given value.

    @param pos The position of the value to set.
    @param value The value to set.
    */
    void set(int pos, type value);

    class StreamWriter {
    private:
        int pos = -1;
        Array<B, N>& ar;

    public:
        /** @brief Sets array reference and resets position.

        @param _ar The array reference.
        */
        void setArray(Array<B, N>& _ar) {
            ar = _ar;
            pos = 0;
        }

        /** @brief Writes next segment to byte array.

        @param result The result byte array.
        @returns Boolean value indicating if whole array has been written.
        */
        bool next(ByteArray<COMM_MSG_DATA_LENGTH>& result);
    };

    friend class StreamWriter;
};

template<int B, int N>
void Array<B, N>::set(int pos, type value) {
    byte container = mask[pos % VPB];
    if (value)
        data[pos / VPB] |= mask[pos % VPB];
    else
        data[pos / VPB] &= ~mask[pos % VPB];
}

bool Array<B, N>::StreamWriter::next(ByteArray<COMM_MSG_DATA_LENGTH>& result) {

    for (int n = 0; n < COMM_MSG_DATA_LENGTH / sizeof(container_type) && pos < N; ++n, ++pos) {
        result[n] = ar.data[pos];
    }

    bool finished = false;

    if (pos == N) {
        pos = 0;
        finished = true;
    }

    return finished;
}
}

#endif // RC_CAR__ARRAY__HPP
