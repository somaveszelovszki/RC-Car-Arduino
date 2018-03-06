#ifndef RC_CAR__ARRAY__HPP
#define RC_CAR__ARRAY__HPP

#include "ByteArray.hpp"

namespace rc_car {

typedef uint8_t uint2_t;
typedef uint8_t uint4_t;

/** @brief Selects type and mask array for given bit length.

@tparam B Number of bits per value.
*/
template <int B>
struct Selector;

#if ARRAY_1_EN
/** @brief Selects type and mask array for bit length of 1.
*/
template <>
struct Selector<1> {
    typedef bool type;
    static const bool min = false;
    static const bool max = true;

    /** @brief Mask for getting and setting bool element at given position in a byte container.
    */
    static const byte mask[8];
};
#endif // ARRAY_1_EN

#if ARRAY_2_EN
/** @brief Selects type and mask array for bit length of 2.
*/
template <>
struct Selector<2> {
    typedef uint2_t type;
    static const uint2_t min = static_cast<uint2_t>(0b00000000);
    static const uint2_t max = static_cast<uint2_t>(0b00000011);

    /** @brief Mask for getting and setting bool element at given position in a byte container.
    */
    static const byte mask[4];
};
#endif // ARRAY_2_EN

#if ARRAY_4_EN
/** @brief Selects type and mask array for bit length of 4.
*/
template <>
struct Selector<4> {
    typedef uint4_t type;
    static const uint4_t min = static_cast<uint4_t>(0b00000000);
    static const uint4_t max = static_cast<uint4_t>(0b00001111);

    /** @brief Mask for getting and setting bool element at given position in a byte container.
    */
    static const byte mask[2];
};
#endif // ARRAY_4_EN

/** @brief Array for storing values requiring less than 8 bits.
Values are stored in a byte array to reduce memory need.

@tparam B Bit length of the stored values (1, 2 or 4).
@tparam N Number of values stored in the array.
NOTE: (B * N) must be a multiple of 8!
@tparam VPB Number of values per byte.
NOTE: Do not set this number explicitly!
*/
template <int B, int N, int VPB = 8 / B>
class Array {
private:
    /** @brief Byte array containing the binary values.
    */
    byte data[N / VPB];

public:
    typedef typename Selector<B>::type type;
    static const type dataMin;
    static const type dataMax;
    static const byte *mask;

    /** @brief Constructor - does not initialize elements.
    */
    Array() {}

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
        const Array<B, N> *pArray;

    public:
        /** @brief Constructor - does not set anything.
        */
        StreamWriter() {}

        /** @brief Sets array reference and resets position.

        @param _pArray Pointer to the array.
        */
        void setArray(const Array<B, N> *_pArray) {
            this->pArray = _pArray;
            this->pos = 0;
        }

        /** @brief Writes next segment to byte array.

        @param result The result byte array.
        @param maxBytesNum Max number of bytes to write (COMM_MSG_DATA_LENGTH by default).
        @param pWrittenPos Will contain the position of the first written byte.
        @returns If whole array has not been written yet: number of bytes written, else: -1 * (number of bytes written).
        */
        int next(ByteArray<COMM_MSG_DATA_LENGTH>& result, int maxBytesNum = COMM_MSG_DATA_LENGTH, int *pWrittenPos = NULL) {

            if (pWrittenPos)
                *pWrittenPos = this->pos;

            int bytesWritten;
            for (bytesWritten = 0; (bytesWritten < maxBytesNum / (int)sizeof(byte)) && (this->pos < N); ++bytesWritten, ++this->pos) {
                result[bytesWritten] = pArray->data[this->pos];
            }

            if (this->pos == N) {
                this->pos = 0;
                bytesWritten *= -1;
            }

            return bytesWritten;
        }
    };

    friend class StreamWriter;

#if _DEBUG
    /** @brief Prints array elements.
    */
    void print() const {
        for (int i = 0; i < N - 1; ++i) {
            DEBUG_print(this->get(i));
            DEBUG_print(" ");
        }

        if (N > 0)
            DEBUG_print(this->get(N - 1));

        DEBUG_println();
    }
#endif // _DEBUG
};

template <int B, int N, int VPB>
const typename Array<B, N, VPB>::type Array<B, N, VPB>::dataMin = Selector<B>::min;

template <int B, int N, int VPB>
const typename Array<B, N, VPB>::type Array<B, N, VPB>::dataMax = Selector<B>::max;

template <int B, int N, int VPB>
const byte *Array<B, N, VPB>::mask = Selector<B>::mask;

template<int B, int N, int VPB>
void Array<B, N, VPB>::set(int pos, type value) {
    if (value)
        data[pos / VPB] |= mask[pos % VPB];
    else
        data[pos / VPB] &= ~mask[pos % VPB];
}
}

#endif // RC_CAR__ARRAY__HPP
