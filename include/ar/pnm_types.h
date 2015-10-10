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
        friend std::ostream& operator<<( std::ostream &out, const pbm_header &header )
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';

            return out;
        }

        friend std::istream& operator>>( std::istream &in, pbm_header &header)
        {
            std::string line;
            bool finished = false, has_magic = false,
                 has_width = false, has_height = false;
            while (!finished)
            {
                in >> std::ws;

                if (in.peek() == '#')
                    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                if (!has_magic && in.peek() == 'P')
                {
                    in.get();
                    header.magic = in.get();

                    if (header.magic != '1' && header.magic != '4')
                        throw std::runtime_error("File does not contain a valid PBM header");

                    has_magic = true;
                    continue;
                }

                if (!has_width)
                {
                    in >> header.width;
                    has_width = true;
                    continue;
                }

                if (!has_height)
                {
                    in >> header.height;
                    has_height = true;
                    finished = true;

                    // read single whitespace ending the header
                    in.get();
                }
            }

            if (!has_magic || !has_width || !has_height)
                throw std::runtime_error("File does not contain a valid PBM header");

            return in;
        }
    };

    struct pgm_header : pnm_header
    {
        friend std::ostream& operator<<( std::ostream &out, const pgm_header &header )
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';
            out << header.max << '\n';

            return out;
        }
    };

    struct ppm_header : pnm_header
    {
        friend std::ostream& operator<<( std::ostream &out, const ppm_header &header )
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

        friend std::ostream& operator<<( std::ostream &out, const pfm_header &header )
        {
            out << 'P' << header.magic << '\n';
            out << header.width << ' ' << header.height << '\n';
            out << (header.endianess * header.scale) << '\n';

            return out;
        }
    };
}

#endif // AR_PNM_TYPES_H
