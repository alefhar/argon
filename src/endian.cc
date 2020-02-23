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

namespace argon
{
    endian endianess()
    {
        std::uint32_t x = 0x42033024;
        char *y = reinterpret_cast<char*>(&x);
        
        if (y[0] == 0x24)
            return endian::LITTLE;

        if (y[0] == 0x42)
            return endian::BIG;

        return endian::UNKNOWN;
    }
    
    std::uint16_t swap(std::uint16_t value)
    {
        std::uint16_t b0 = value & 255;
        std::uint16_t b1 = (value >> 8) & 255;

        return (b0 << 8) + b1;
    }

    std::uint16_t no_swap(std::uint16_t value)
    {
        return value;
    }

    std::uint32_t swap(std::uint32_t value)
    {
        std::uint32_t b0 = value & 255;
        std::uint32_t b1 = (value >>  8) & 255;
        std::uint32_t b2 = (value >> 16) & 255;
        std::uint32_t b3 = (value >> 24) & 255;

        return (b0 << 24) + (b1 << 16) + (b2 << 8) + b3;
    }

    std::uint32_t no_swap(std::uint32_t value)
    {
        return value;
    }

    float swap(float value)
    {
        std::uint8_t *b = reinterpret_cast<std::uint8_t *>(&value);
        
        std::uint8_t f[4];
        f[0] = b[3];
        f[1] = b[2];
        f[2] = b[1];
        f[3] = b[0];

        // GCC displays a message that the cast from
        // &f[0] violates strict aliasing, so an
        // intermediate step is necessary.
        std::uint8_t *f_ptr = &f[0];        
        return *reinterpret_cast<float *>(f_ptr);
    }

    float no_swap(float value)
    {
        return value;
    }
}
