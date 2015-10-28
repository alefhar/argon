#ifndef AR_PNM_TYPES_H
#define AR_PNM_TYPES_H

#include "ar/image.h"

#include <string>
#include <iostream>
#include <limits>

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
        public:
            char     magic;
            pnm_type type;
            int      width;
            int      height;

        protected:
            virtual void check();
    };

    struct pnm_header : header_data
    {
        public:
            int max;
            int bytes;

        protected:
            virtual void check();
    };

    struct pbm_header : header_data
    {
        public:
            int max;

            pbm_header()
            {
                max = 1;
            }

            friend std::ostream& operator<<( std::ostream &out, const pbm_header &header );       
            friend std::istream& operator>>( std::istream &in, pbm_header &header);

        protected:
            virtual void check();
    };

    struct pgm_header : pnm_header
    {
        friend std::ostream& operator<<( std::ostream &out, const pgm_header &header );
        friend std::istream& operator>>( std::istream &in, pgm_header &header);

        protected:
            virtual void check();
    };

    struct ppm_header : pnm_header
    {
        friend std::ostream& operator<<( std::ostream &out, const ppm_header &header );
        friend std::istream& operator>>( std::istream &in, ppm_header &header);

        protected:
            virtual void check();
    };

    struct pfm_header : header_data
    {
        public:
            float endianess;
            float scale;

            friend std::ostream& operator<<( std::ostream &out, const pfm_header &header );
            friend std::istream& operator>>( std::istream &in, pfm_header &header );

        protected:
            virtual void check();
    };
}

#endif // AR_PNM_TYPES_H
