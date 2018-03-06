#ifndef RC_CAR__GRID__HPP
#define RC_CAR__GRID__HPP

#include "Array.hpp"

namespace rc_car {

/** @brief Grid (2-dimensional array) of values requiring less than 8 bits.
Values are stored in byte arrays to reduce memory need.

@tparam B Bit length of the stored values (1, 2 or 4).
@tparam X Number of values stored in the X axis.
@tparam Y Number of values stored in the Y axis.
NOTE: (Y * N) must be a multiple of 8!
@tparam VPB Stored values per byte.
NOTE: Do not set this parameter explicitly!
*/
template <int B, int X, int Y>
class Grid {
private:
    /** @brief Type of the arrays that stores the horizontal rows of the grid.
    */
    typedef Array<B, X> array_type;

    /** @brief Array of byte arrays containing the binary values.
    */
    array_type data[Y];

public:
    typedef typename array_type::type type;
    static const type dataMin;
    static const type dataMax;

    /** @brief Constructor - does not initialize elements.
    */
    Grid() {}

    /** @brief Gets value at given X,Y coordinate.

    @param x The X coordinate of the value to get.
    @param y The Y coordinate of the value to get.
    @returns The value of the element.
    */
    type get(int x, int y) const {
        return this->data[y].get(x);
    }

    /** @brief Sets element at given X,Y coordinate to the given value.

    @param x The X coordinate of the value to set.
    @param y The Y coordinate of the value to set.
    @param value The value to set.
    */
    void set(int x, int y, type value) {
        this->data[y].set(x, value);
    }

    void increment(int x, int y);

    void decrement(int x, int y);

    class StreamWriter {
    private:
        int y = -1;
        const Grid<B, X, Y> *pGrid;
        typename array_type::StreamWriter arrayWriter;

    public:
        /** @brief Constructor - does not set anything.
        */
        StreamWriter() {}

        /** @brief Sets grid reference and resets position.

        @param _pGrid Pointer to the grid.
        */
        void setGrid(const Grid<B, X, Y> *_pGrid) {
            this->pGrid = _pGrid;
            this->y = 0;
            this->arrayWriter.setArray(&_pGrid->data[0]);
        }

        /** @brief Writes next segment to byte array.

        @param result The result byte array.
        @param maxBytesNum Max number of bytes to write (COMM_MSG_DATA_LENGTH by default).
        @param pWrittenX The X coordinate of the first written byte.
        @param pWrittenY The Y coordinate of the first written byte.
        @returns If whole grid has not been written yet: number of bytes written, else: -1 * (number of bytes written).
        */
        int next(ByteArray<COMM_MSG_DATA_LENGTH>& result, int maxBytesNum = COMM_MSG_DATA_LENGTH, int *pWrittenX = NULL, int *pWrittenY = NULL) {

            if (pWrittenY)
                *pWrittenY = this->y;

            int bytesWritten = this->arrayWriter.next(result, maxBytesNum, pWrittenX);
            if (bytesWritten < 0) {     // whole row array has been written

                bool finished = ++y == Y;
                if (finished) {
                    // bytesWritten is already negative, no need to change it
                    this->y = 0;
                }

                this->arrayWriter.setArray(&this->pGrid->data[y]);

                if (!finished && ((bytesWritten *= -1) < maxBytesNum)) {    // not all data bytes have been filled yet
                    ByteArray<COMM_MSG_DATA_LENGTH> sub(&result[bytesWritten]);
                    bytesWritten += this->arrayWriter.next(sub, maxBytesNum - bytesWritten);
                }
            }

            return bytesWritten;
        }
    };

    friend class StreamWriter;

#if _DEBUG
    /** @brief Prints grid elements.
    */
    void print() const {
        for (int y = 0; y < Y; ++y)
            this->data[y].print();
    }
#endif // _DEBUG
};

template<int B, int X, int Y>
const typename Grid<B, X, Y>::type Grid<B, X, Y>::dataMin = Array<B, X>::dataMin;

template<int B, int X, int Y>
const typename Grid<B, X, Y>::type Grid<B, X, Y>::dataMax = Array<B, X>::dataMax;

template<int B, int X, int Y>
void Grid<B, X, Y>::increment(int x, int y) {
    type value = get(x, y);
    if (value < dataMax)
        set(x, y, value + 1);
}

template<int B, int X, int Y>
void Grid<B, X, Y>::decrement(int x, int y) {
    type value = get(x, y);
    if (value > dataMin)
        set(x, y, value - 1);
}
}

#endif // RC_CAR__GRID__HPP
