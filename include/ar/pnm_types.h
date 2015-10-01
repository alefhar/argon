#ifndef AR_PNM_TYPES_H
#define AR_PNM_TYPES_H

#include "ar/image.h"

#include <iostream>

namespace argon
{
    enum class pnm_type : char
    { 
        PBM_ASCII  = '1', PGM_ASCII  = '2', PPM_ASCII  = '3',
        PBM_BINARY = '4', PGM_BINARY = '5', PPM_BINARY = '6',
        PFM_SINGLE = 'f', PFM_TRIPLE = 'F', PFM_ANY    = 'X'
    };

    struct header_data
    {
        char magic;
        int  width;
        int  height;
    };

    struct pnm_header : header_data
    {
        int max;
        int bytes;
    };

    struct pbm_header : pnm_header
    {
        friend std::ostream& operator<<(std::ostream &out, const pbm_header &header)
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';

            return out;
        }
    };

    struct pgm_header : pnm_header
    {
        friend std::ostream& operator<<(std::ostream &out, const pgm_header &header)
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';
            out << header.max << '\n';

            return out;
        }
    };

    struct ppm_header : pnm_header
    {
        friend std::ostream& operator<<(std::ostream &out, const ppm_header &header)
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';
            out << header.max << '\n';

            return out;
        }
    };

    struct pfm_header : header_data
    {
        float endianess;
        float scale;

        friend std::ostream& operator<<(std::ostream &out, const pfm_header &header)
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';
            out << (header.endianess * header.scale) << '\n';

            return out;
        }
    };
}

#endif // AR_PNM_TYPES_H
