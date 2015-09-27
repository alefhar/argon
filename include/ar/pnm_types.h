#ifndef AR_PNM_TYPES_H
#define AR_PNM_TYPES_H

#include "ar/image.h"

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
        const int allowed_max = 1;
    };

    struct pgm_header : pnm_header
    {
        const int allowed_max = 65535;
    };

    struct ppm_header : pnm_header
    {
        const int allowed_max = 65535;
    };

    struct pfm_header : header_data
    {
        float endianess;
        float scale;
    };
}

#endif // AR_PNM_TYPES_H
