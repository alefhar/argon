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
