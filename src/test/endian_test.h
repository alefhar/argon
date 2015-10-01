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
