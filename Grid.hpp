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
template <int B, int X, int Y, int VPB = 8 / B>
class Grid {
private:
    /** @brief Array of byte arrays containing the binary values.
    */
    Array<B, Y> data[X];

public:
    typedef Array<B, Y>::type type;

    /** @brief Gets value at given X,Y coordinate.

    @param x The X coordiante of the value to get.
    @param y The Y coordiante of the value to get.
    @returns The value of the element.
    */
    type get(int x, int y) const {
        return data[x].get(y);
    }

    /** @brief Sets element at given X,Y coordinate to the given value.

    @param x The X coordiante of the value to set.
    @param y The Y coordiante of the value to set.
    @param value The value to set.
    */
    void set(int x, int y, type value) {
        data[x].set(y, value);
    }
};
}

#endif // RC_CAR__GRID__HPP
