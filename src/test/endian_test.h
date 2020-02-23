// This file is part of libargon.
//
// libargon is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libargon is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with libargon.  If not, see <http://www.gnu.org/licenses/>.

#include "ar/endian.h"

#include <cstdint>

TEST (endian_test, swap_short)
{
    std::uint16_t val[3] = { 0x0042, 0x1417, 0x2390 };
    std::uint16_t exp[3] = { 0x4200, 0x1714, 0x2390 };

    EXPECT_EQ(exp[0], argon::swap(val[0]));
    EXPECT_EQ(exp[1], argon::swap(val[1]));
    EXPECT_EQ(exp[2], argon::swap(argon::swap(val[2])));
}

TEST (endian_test, swap_int)
{
    std::uint32_t val[3] = { 0x00000042, 0x55e18f42, 0xa8b1deff };
    std::uint32_t exp[3] = { 0x42000000, 0x428fe155, 0xa8b1deff };

    EXPECT_EQ(exp[0], argon::swap(val[0]));
    EXPECT_EQ(exp[1], argon::swap(val[1]));
    EXPECT_EQ(exp[2], argon::swap(argon::swap(val[2])));
}

TEST (endian_test, swap_float)
{
    float f, swapped, swapped_twice;
    
    f = 42.f;
    swapped = argon::swap(f);
    swapped_twice = argon::swap(swapped);

    EXPECT_EQ(f, swapped_twice);

    f = -1359.8471f;
    swapped = argon::swap(f);
    swapped_twice = argon::swap(swapped);

    EXPECT_EQ(f, swapped_twice);
}
