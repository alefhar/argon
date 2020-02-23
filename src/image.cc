#include "ar/image.h"

namespace argon
{
    bool operator==(const image<float> &lhs, const image<float> &rhs)
    {
        if (lhs._width != rhs._width  ||
                lhs._height != rhs._height ||
                lhs._channels != rhs._channels)
            return false;

        const std::int32_t ULP = 5;
        float *xf, *yf;
        std::int32_t *xi, *yi;
        for (auto p = 0u; p < lhs._data.size(); ++p)
        {
            *xf = lhs._data[p];
            *yf = rhs._data[p];

            if (*xf != *yf)
            {
                xi = reinterpret_cast<std::int32_t *>(xf);
                yi = reinterpret_cast<std::int32_t *>(yf);
                std::int32_t int_diff = std::abs(*xi - *yi);
                if (int_diff > ULP)
                    return false;
            }
        }

        return true;
    }

    bool operator==(const image<double> &lhs, const image<double> &rhs)
    {
        if (lhs._width != rhs._width  ||
                lhs._height != rhs._height ||
                lhs._channels != rhs._channels)
            return false;

        const std::int64_t ULP = 8;
        double *xf, *yf;
        std::int64_t *xi, *yi;
        for (auto p = 0u; p < lhs._data.size(); ++p)
        {
            *xf = lhs._data[p];
            *yf = rhs._data[p];

            if (*xf != *yf)
            {
                xi = reinterpret_cast<std::int64_t *>(xf);
                yi = reinterpret_cast<std::int64_t *>(yf);
                std::int64_t int_diff = std::abs(*xi - *yi);
                if (int_diff > ULP)
                    return false;
            }
        }

        return true;
    }
    
    bool operator!=( const image<float> &lhs, const image<float> &rhs )
    {
        return !(lhs == rhs);
    }

    bool operator!=( const image<double> &lhs, const image<double> &rhs )
    {
        return !(lhs == rhs);
    }
}
