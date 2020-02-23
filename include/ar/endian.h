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

#ifndef AR_ENDIAN_H
#define AR_ENDIAN_H

#include <cstdint>

namespace argon
{
    enum class endian { LITTLE, BIG, UNKNOWN };

    endian endianess();
    
    std::uint16_t swap(std::uint16_t value);

    std::uint16_t no_swap(std::uint16_t value);

    std::uint32_t swap(std::uint32_t value);

    std::uint32_t no_swap(std::uint32_t value);

    float swap(float value);

    float no_swap(float value);
}

#endif // AR_ENDIAN_H
