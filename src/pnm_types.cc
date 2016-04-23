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

#include "ar/pnm_types.h"
#include "ar/pnm_header_parser.h"

#include <stdexcept>

namespace argon
{
    void header_data::check()
    {
        if (width <= 0)
            throw std::runtime_error("Unable to parse header, invalid width.");

        if (height <= 0)
            throw std::runtime_error("Unable to parse header, invalid height.");
    }

    void pnm_header::check()
    {
        header_data::check();

        if (max <= 0 || max > 65535)
            throw std::runtime_error("Unable to parse header, invalid maximum.");
    }

    void pbm_header::check()
    {
        header_data::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PBM_ASCII && type != pnm_type::PBM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid PBM magic.");
    }

    void pgm_header::check()
    {
        pnm_header::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PGM_ASCII && type != pnm_type::PGM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid PGM magic.");
    }

    void ppm_header::check()
    {
        pnm_header::check();

        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PPM_ASCII && type != pnm_type::PPM_BINARY)
            throw std::runtime_error("Unable to parse header, invalid PPM magic.");
    }

    void pfm_header::check()
    {
        header_data::check();
        
        pnm_type type = static_cast<pnm_type>(magic);
        if (type != pnm_type::PFM_SINGLE && type != pnm_type::PFM_TRIPLE)
            throw std::runtime_error("Unable to parse header, invalid PFM magic.");

        if (endianess != 1.f && endianess != -1.f)
            throw std::runtime_error("Unable to parse header, invalid endianess.");
    }

    std::ostream& operator<<( std::ostream &out, const pbm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, pbm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_pbm_header(in, header);
        
        header.check();
        
        header.type  = static_cast<pnm_type>(header.magic); 

        return in;
    }
        
    std::ostream& operator<<( std::ostream &out, const pgm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << header.max << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, pgm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_pgm_header(in, header);

        header.check();

        header.type  = static_cast<pnm_type>(header.magic); 
        header.bytes = header.max <= 255 ? 1 : 2;

        return in;
    }

    std::ostream& operator<<( std::ostream &out, const ppm_header &header )
    {
        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << header.max << '\n';

        return out;
    }

    std::istream& operator>>( std::istream &in, ppm_header &header)
    {
        pnm_header_parser parser;
        parser.parse_ppm_header(in, header);

        header.check();
        
        header.type  = static_cast<pnm_type>(header.magic); 
        header.bytes = header.max <= 255 ? 1 : 2;

        return in;
    }
            
    std::ostream& operator<<( std::ostream &out, const pfm_header &header )
    {

        out << 'P' << header.magic << '\n';
        out << header.width << ' ' << header.height << '\n';
        out << (header.endianess * header.scale) << '\n';

        return out;
    }
            
    std::istream& operator>>( std::istream &in, pfm_header &header )
    {
        pnm_header_parser parser;
        parser.parse_pfm_header(in, header);

        header.check();

        header.type  = static_cast<pnm_type>(header.magic); 
        
        return in;
    }
}
